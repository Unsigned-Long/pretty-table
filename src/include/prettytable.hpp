#pragma once

#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace ns_pretab {

#define THROW_EXCEPTION(str) throw std::runtime_error(std::string("[ error from 'pretty-table' ] ") + str)

  enum class TabAlign {
    /**
     * @brief options
     */
    RIGHT,
    LEFT,
    CENTER
  };

  namespace ns_priv {
    /**
     * @brief override operator '<<' for type 'TabAlign'
     */
    static std::ostream &operator<<(std::ostream &os, const TabAlign &obj) {
      switch (obj) {
      case TabAlign::RIGHT:
        os << "right";
        break;
      case TabAlign::LEFT:
        os << "left";
        break;
      case TabAlign::CENTER:
        os << "center";
        break;
      }
      return os;
    };

    /**
     * @brief the column data structure for a table
     */
    class TabColumn : public std::vector<std::string> {
    public:
      friend class TabDataModel;

    private:
      // used when drawing the table
      std::size_t _maxWidth;
      // the header label
      std::string _header;
      // the align
      TabAlign _align;
      // the precision for float values
      std::size_t _precision;

    public:
      TabColumn(const std::string &header,
                TabAlign align = TabAlign::CENTER,
                std::size_t precision = 1)
          : _header(header), _maxWidth(header.size()),
            std::vector<std::string>(), _align(align),
            _precision(precision) {}

      /**
       * @brief add a item at the end of the column
       *
       * @param item the item string
       */
      void push_back(const std::string &item) {
        std::vector<std::string>::push_back(item);
        if (item.size() > this->_maxWidth) {
          this->_maxWidth = item.size();
        }
        return;
      }

#pragma region attributes
      /**
       * @brief get header string
       *
       * @return const std::string&
       */
      const std::string &header() const {
        return this->_header;
      }

      /**
       * @brief get the align of this table column
       *
       * @return TabAlign
       */
      TabAlign align() const {
        return this->_align;
      }

      std::size_t maxWidth() const {
        return this->_maxWidth;
      }

      std::size_t precision() const {
        return this->_precision;
      }

#pragma endregion

      std::string info() const {
        std::stringstream stream;
        stream << "{'" << this->_header << "': " << this->_align << "}";
        return stream.str();
      }

    private:
      TabColumn() = delete;
    };

    class TabDataModel {
    protected:
      // the table data
      std::vector<TabColumn> _data;

    public:
      /**
       * @brief Construct a new TabDataModel object
       *
       * @param headers the header labels for the table
       */
      TabDataModel(const std::vector<std::string> &headers) {
        for (const auto &elem : headers) {
          this->_data.push_back(TabColumn(elem));
        }
      }

      TabDataModel() = default;

#pragma region table state
      /**
       * @brief is the table empty
       *
       * @return true
       * @return false
       */
      bool empty() const {
        return this->_data.empty();
      }

      /**
       * @brief the row count
       *
       * @return std::size_t
       */
      std::size_t rowCount() const {
        if (this->_data.empty()) {
          return 0;
        } else {
          return this->_data.front().size();
        }
      }

      /**
       * @brief the column count
       *
       * @return std::size_t
       */
      std::size_t columnCount() const {
        return this->_data.size();
      }
#pragma endregion

#pragma region get and set for 'header' 'align' 'precision'
      /**
       * @brief get the header label at 'column'
       *
       * @param column
       * @return std::string&
       */
      const std::string &headerAt(std::size_t column) const {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'headerAt'");
        }
        return this->_data.at(column)._header;
      }

      /**
       * @brief Set the Header at 'column'
       *
       * @return TabDataModel&
       */
      TabDataModel &setHeader(std::size_t column, const std::string header) {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'setHeader'");
        }
        this->_data.at(column)._header = header;
        if (header.size() > this->_data.at(column)._maxWidth) {
          this->_data.at(column)._maxWidth = header.size();
        }
        return *this;
      }

      /**
       * @brief get the align at 'column'
       *
       * @param column
       * @return TabAlign&
       */
      const TabAlign &alignAt(std::size_t column) const {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'alignAt'");
        }
        return this->_data.at(column)._align;
      }

      /**
       * @brief Set the Align at 'column'
       *
       * @return TabDataModel&
       */
      TabDataModel &setAlign(std::size_t column, TabAlign align) {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'setAlign'");
        }
        this->_data.at(column)._align = align;
        return *this;
      }

      /**
       * @brief Set the Align for all columns
       *
       * @return TabDataModel&
       */
      TabDataModel &setAlign(TabAlign align) {
        for (auto &elem : this->_data) {
          elem._align = align;
        }
        return *this;
      }

      /**
       * @brief get the precision at 'column'
       *
       * @return std::size_t&
       */
      const std::size_t &precisionAt(std::size_t column) const {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'precisionAt'");
        }
        return this->_data.at(column)._precision;
      }

      /**
       * @brief Set the Precision at 'column'
       *
       * @return TabDataModel&
       */
      TabDataModel &setPrecision(std::size_t column, std::size_t precision) {
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'setPrecision'");
        }
        this->_data.at(column)._precision = precision;
        return *this;
      }

      /**
       * @brief Set the Precision for all columns
       *
       * @return TabDataModel&
       */
      TabDataModel &setPrecision(std::size_t precision) {
        for (auto &elem : this->_data) {
          elem._precision = precision;
        }
        return *this;
      }

#pragma endregion

#pragma region change data in the table
      /**
       * @brief Set the Item at [row, column]
       *
       * @return TabDataModel&
       */
      template <typename ItemType>
      TabDataModel &setItem(const ItemType &item, std::size_t row, std::size_t column) {
        if (row >= this->rowCount()) {
          THROW_EXCEPTION("the param 'row' is invalid in 'setItem'");
        }
        if (column >= this->columnCount()) {
          THROW_EXCEPTION("the param 'column' is invalid in 'setItem'");
        }
        std::stringstream stream;
        stream << std::fixed << std::setprecision(this->_data.at(column)._precision) << item;
        this->_data.at(column).at(row) = stream.str();
        if (stream.str().size() > this->_data.at(column)._maxWidth) {
          this->_data.at(column)._maxWidth = stream.str().size();
        }
        return *this;
      }

      /**
       * @brief add a row to the end of the table
       *
       * @tparam ArgvsType the types of the argvs
       * @param argvs the arguements
       * @return TabDataModel&
       */
      template <typename... ArgvsType>
      TabDataModel &appendRow(const ArgvsType &...argvs) {
        if (sizeof...(argvs) != this->columnCount()) {
          THROW_EXCEPTION("the count of 'argvs' is invalid in 'appendRow'");
        }
        return this->__appendRow__(0, argvs...);
      }

      /**
       * @brief add a column to the end of the table
       *
       * @return TabDataModel&
       */
      TabDataModel &appendColumn(const std::string &header,
                                 TabAlign align = TabAlign::CENTER,
                                 std::size_t precision = 1) {
        this->_data.push_back(TabColumn(header, align, precision));
        this->_data.back().resize(this->rowCount());
        return *this;
      }
#pragma endregion

#pragma region help functions
    protected:
      /**
       * @brief help function
       */
      template <typename ArgvType, typename... ArgvsType>
      TabDataModel &__appendRow__(std::size_t columnIndex, const ArgvType &argv, const ArgvsType &...argvs) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(this->precisionAt(columnIndex)) << argv;
        this->_data.at(columnIndex).push_back(stream.str());
        return this->__appendRow__(columnIndex + 1, argvs...);
      }

      /**
       * @brief help function
       */
      TabDataModel &__appendRow__(std::size_t columnIndex) {
        return *this;
      }

      const TabColumn &columnAt(std::size_t column) const {
        return this->_data.at(column);
      }

      /**
       * @brief get the item at [row, column]
       *
       * @param row
       * @param column
       * @return std::string&
       */
      const std::string &itemAt(std::size_t row, std::size_t column) const {
        return this->_data.at(column).at(row);
      }

#pragma endregion
    };

    class TabPrinter;

  } // namespace ns_priv

  class PrettyTable : public ns_priv::TabDataModel {

  public:
    friend class ns_priv::TabPrinter;
    using TabDataModel::TabDataModel;

    /**
     * @brief translate the table to csv file format
     *
     * @param splitor the splitor
     * @return std::string
     */
    std::string toCSV(char splitor = ',') const {
      std::stringstream stream;
      // print the headers
      for (int i = 0; i != this->columnCount() - 1; ++i) {
        stream << this->_data.at(i).header() << splitor;
      }
      stream << this->_data.back().header() << '\n';
      // print the data
      for (int i = 0; i != this->rowCount(); ++i) {
        for (int j = 0; j != this->columnCount() - 1; ++j) {
          stream << this->itemAt(i, j) << splitor;
        }
        stream << this->itemAt(i, this->columnCount() - 1) << '\n';
      }
      return stream.str();
    }

    /**
     * @brief load the csv file data to comstruct a table
     *
     * @param filename the name of csv file
     * @param columnCount the count of columns
     * @param withHeaders is the csv file with headers
     * @param splitor the splitor
     * @return PrettyTable
     */
    static PrettyTable fromCSV(const std::string &filename, std::size_t columnCount,
                               std::size_t precision = 1, bool withHeaders = true, char splitor = ',') {
      std::ifstream ifs(filename);
      if (!ifs.is_open()) {
        THROW_EXCEPTION("the csv file is not found in 'fromCSV'");
      }
      std::vector<std::string> headers;
      std::string strLine;
      // organize the headers
      if (!withHeaders) {
        headers.resize(columnCount);
        std::size_t columnIndex = 0;
        for (auto &elem : headers) {
          elem = "Column(" + std::to_string(columnIndex++) + ")";
        }
      } else {
        std::getline(ifs, strLine);
        if (strLine.empty()) {
          return PrettyTable(std::vector<std::string>());
        } else {
          headers = PrettyTable::split(strLine, splitor);
          for (int i = 0; i < columnCount - headers.size(); ++i) {
            headers.push_back("No-Header");
          }
        }
      }
      // organize the data
      PrettyTable tab(headers);

      while (std::getline(ifs, strLine)) {
        auto items = PrettyTable::split(strLine, splitor);
        for (int i = 0; i != std::min(items.size(), columnCount); ++i) {
          tab._data.at(i).push_back(items.at(i));
        }
        for (int i = 0; i < columnCount - items.size(); ++i) {
          tab._data.at(i + std::min(items.size(), columnCount)).push_back("No-Data");
        }
      }
      ifs.close();
      return tab;
    }

    /**
     * @brief translate the table to mark down format
     *
     * @return std::string
     */
    std::string toMarkDown() const {
      std::stringstream stream;
      // print headers
      stream << '|';
      for (int i = 0; i != this->columnCount(); ++i) {
        stream << this->_data.at(i).header() << '|';
      }
      stream << "\n|";
      // print split line
      for (int i = 0; i != this->columnCount(); ++i) {
        stream << "----|";
      }
      stream << '\n';
      // print data
      for (int i = 0; i != this->rowCount(); ++i) {
        stream << '|';
        for (int j = 0; j != this->columnCount(); ++j) {
          stream << this->itemAt(i, j) << "|";
        }
        stream << "\n";
      }
      return stream.str();
    }

    /**
     * @brief add a row at the end of the table
     *
     * @tparam ArgvsType
     * @param argvs
     * @return PrettyTable&
     */
    template <typename... ArgvsType>
    PrettyTable &appendRow(const ArgvsType &...argvs) {
      ns_priv::TabDataModel::appendRow(argvs...);
      return *this;
    }

    /**
     * @brief print the table's info
     *
     * @return std::string
     */
    std::string tableInfo() const {
      std::stringstream stream;
      stream << "{'rows': " << this->rowCount();
      stream << ", 'colms': " << this->columnCount();
      stream << ", 'columns': [";
      for (int i = 0; i != this->columnCount(); ++i) {
        stream << this->columnAt(i).info();
        if (i != this->columnCount() - 1)
          stream << ", ";
      }
      stream << "]}";

      return stream.str();
    }

  protected:
    /**
     * \brief a function to split a string to some string elements according the splitor
     * \param str the string to be splited
     * \param splitor the splitor char
     * \param ignoreEmpty whether ignoring the empty string element or not
     * \return the splited string vector
     */
    static std::vector<std::string> split(const std::string &str, char splitor, bool ignoreEmpty = true) {
      std::vector<std::string> vec;
      auto iter = str.cbegin();
      while (true) {
        auto pos = std::find(iter, str.cend(), splitor);
        auto elem = std::string(iter, pos);
        if ((!ignoreEmpty) || (ignoreEmpty && !elem.empty()))
          vec.push_back(elem);
        if (pos == str.cend())
          break;
        iter = ++pos;
      }
      return vec;
    }
  };

  namespace ns_priv {

    class TabPrinter {
    public:
      TabPrinter() = default;

      std::ostream &operator()(std::ostream &os, const PrettyTable &pretab) {
        if (pretab.empty()) {
          return (*this)(os, PrettyTable({"empty"}));
        }
        auto line = this->split_line(pretab);
        os << line << std::endl;
        this->print_head(os, pretab) << std::endl;
        os << line << std::endl;
        this->print_data(os, pretab, line);
        return os;
      }

    protected:
      std::string split_line(const PrettyTable &pretab) {
        std::stringstream stream;
        stream << '+';
        for (int i = 0; i != pretab.columnCount(); ++i) {
          stream << std::string(pretab.columnAt(i).maxWidth() + 2, '-') << '+';
        }
        return stream.str();
      }

      std::ostream &print_head(std::ostream &os, const PrettyTable &pretab) {
        os << '|';
        for (int i = 0; i != pretab.columnCount(); ++i) {
          auto &colm = pretab.columnAt(i);
          this->print_item(os, colm.header(), colm.maxWidth(), TabAlign::CENTER) << '|';
        }
        return os;
      }

      std::ostream &print_data(std::ostream &os, const PrettyTable &pretab, const std::string &line) {
        for (int j = 0; j != pretab.rowCount(); ++j) {
          os << '|';
          for (int i = 0; i != pretab.columnCount(); ++i) {
            auto &colm = pretab.columnAt(i);
            this->print_item(os, colm.at(j), colm.maxWidth(), colm.align()) << '|';
          }
          os << '\n';
          os << line << '\n';
        }
        return os;
      }

      std::ostream &print_item(std::ostream &os, const std::string &content, int colmWidth, TabAlign align) {
        os << ' ';
        switch (align) {
        case TabAlign::CENTER: {
          int left_space = (colmWidth - content.size()) / 2;
          int right_space = colmWidth - left_space - content.size();
          os << std::string(left_space, ' ') << content << std::string(right_space, ' ');
        } break;
        case TabAlign::LEFT: {
          os << std::left << std::setw(colmWidth) << content;
        } break;
        case TabAlign::RIGHT: {
          os << std::right << std::setw(colmWidth) << content;
        } break;
        }
        os << ' ';
        return os;
      }
    } printer;

    /**
     * @brief output format for the pretty table
     *
     * @param os the ostream
     * @param pretab the pretty table
     * @return std::ostream&
     */
    static std::ostream &operator<<(std::ostream &os, const PrettyTable &pretab) {
      return ns_priv::printer(os, pretab);
    }

  } // namespace ns_priv

} // namespace ns_pretab
