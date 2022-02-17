#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ns_pretab::ns_priv {

  struct TableColumn {
  private:
    /**
     * @brief the members
     */
    std::string _header;
    std::vector<std::string> _data;
    std::size_t _max_width;

  public:
    /**
     * @brief construct a new TableColumn object
     */
    TableColumn() = default;

    TableColumn(const std::string &header, const std::vector<std::string> &data)
        : _header(header), _data(data), _max_width(header.size()) {}

    inline std::string &header() { return this->_header; }
    inline const std::string &header() const { return this->_header; }

    inline std::vector<std::string> &data() { return this->_data; }
    inline const std::vector<std::string> &data() const { return this->_data; }

    inline std::size_t get_max_width() const { return this->_max_width; }

    inline void set_max_width(std::size_t max_width) { this->_max_width = max_width; }

    inline void refind_max_width() {
      this->_max_width = this->_header.size();
      for (const auto &elem : this->_data)
        if (this->_max_width < elem.size())
          this->_max_width = elem.size();
      return;
    }
  };

} // namespace ns_pretab::ns_priv

namespace ns_pretab {

  enum class TabAlign {
    /**
     * @brief options
     */
    RIGHT,
    LEFT,
    CENTER
  };

  /**
   * @brief override operator '<<' for type 'TabAlign'
   */
  static std::ostream &operator<<(std::ostream &os, const TabAlign &obj) {
    switch (obj) {
    case TabAlign::RIGHT:
      os << "RIGHT";
      break;
    case TabAlign::LEFT:
      os << "LEFT";
      break;
    case TabAlign::CENTER:
      os << "CENTER";
      break;
    }
    return os;
  };

  class PrettyTable {
  private:
    /**
     * @brief members
     */
    std::vector<ns_priv::TableColumn> _tab;

    TabAlign _align;

    std::size_t _precision;

    std::size_t _rows;

  public:
#pragma region constructors
    /**
     * @brief Construct a new PrettyTable object
     * @param align the align
     * @param precision the precision
     */
    PrettyTable(TabAlign align = TabAlign::CENTER, std::size_t precision = 3)
        : _align(align), _precision(precision), _rows(0) {}

    /**
     * @brief Construct a new PrettyTable object
     *
     * @param headers the header labels
     * @param align the align
     * @param precision the precision
     */
    PrettyTable(const std::vector<std::string> &headers,
                TabAlign align = TabAlign::CENTER, std::size_t precision = 3)
        : _align(align), _precision(precision), _rows(0) {
      this->_tab.resize(headers.size());
      for (int i = 0; i != headers.size(); ++i) {
        this->_tab.at(i).header() = headers.at(i);
        this->_tab.at(i).set_max_width(headers.at(i).size());
      }
    }

#pragma endregion

#pragma region 'set' or 'get' operator
    /**
     * @brief Set the precision for float value
     *
     * @param precision the precision
     */
    inline void set_precision(std::size_t precision) {
      this->_precision = precision;
      return;
    }

    /**
     * @brief Set the align
     *
     * @param align the align
     */
    inline void set_align(TabAlign align) {
      this->_align = align;
      return;
    }

    /**
     * @brief get the align of this table
     *
     * @return TabAlign
     */
    inline TabAlign get_align() const { return this->_align; }

    /**
     * @brief Get the precision
     *
     * @return std::size_t
     */
    inline std::size_t get_precision() const { return this->_precision; }

    /**
     * @brief get the table
     *
     * @return const std::vector<TableColumn>&
     */
    inline const std::vector<ns_priv::TableColumn> &get_table() const {
      return this->_tab;
    }

#pragma endregion

#pragma region 'row' or 'column' operator

    /**
     * @brief add a column at the end of this table
     *
     * @param header the header label of this column
     * @return PrettyTable&
     */
    inline PrettyTable &append_colum(const std::string &header) {
      this->insert_colum(header, this->colms());
      return *this;
    }

    /**
     * @brief add a column at the 'colm_index' of this table
     *
     * @param header the header table
     * @param colm_index the index of the column
     * @return PrettyTable&
     */
    inline PrettyTable &insert_colum(const std::string &header, int colm_index) {
      auto new_colm_data = std::vector<std::string>(this->rows());
      if (colm_index < 0 || colm_index >= this->_tab.size())
        this->_tab.push_back(ns_priv::TableColumn(header, new_colm_data));
      else
        this->_tab.insert(this->_tab.cbegin() + colm_index,
                          ns_priv::TableColumn(header, new_colm_data));
      return *this;
    }

    /**
     * @brief add some columns at the end of this table
     *
     * @param headers
     * @return PrettyTable&
     */
    inline PrettyTable &append_colums(const std::vector<std::string> &headers) {
      this->insert_colums(headers, this->colms());
      return *this;
    }

    /**
     * @brief add a column at the 'colm_index' of this table
     *
     * @param headers the header labels
     * @param colm_index the index of the column to start inserting
     * @return PrettyTable&
     */
    PrettyTable &insert_colums(const std::vector<std::string> &headers,
                               int colm_index) {
      // organize the table column
      std::vector<ns_priv::TableColumn> new_colms(headers.size());
      for (int i = 0; i != headers.size(); ++i) {
        new_colms.at(i).header() = headers.at(i);
        new_colms.at(i).set_max_width(headers.at(i).size());
        new_colms.at(i).data().resize(this->rows());
      }
      // check the param 'colm_index'
      if (colm_index < 0 || colm_index >= this->_tab.size()) {
        // insert at the end
        this->_tab.insert(this->_tab.cend(), new_colms.cbegin(),
                          new_colms.cend());
      } else {
        // insert at the 'colm_index'
        this->_tab.insert(this->_tab.cbegin() + colm_index, new_colms.cbegin(),
                          new_colms.cend());
      }
      return *this;
    }

    /**
     * @brief add a row to this table at the end
     *
     * @tparam ArgType the type of the arg
     * @tparam ArgsType the types of the args
     * @param arg
     * @param args
     * @return PrettyTable&
     */
    template <typename ArgType, typename... ArgsType>
    inline PrettyTable &append_row(const ArgType &arg, const ArgsType &...args) {
      this->insert_row(this->rows(), arg, args...);
      return *this;
    }

    /**
     * @brief add a row to this table at the 'row_index'
     *
     * @tparam ArgType the type of the arg
     * @tparam ArgsType the types of the args
     * @param arg
     * @param args
     * @return PrettyTable&
     */
    template <typename ArgType, typename... ArgsType>
    PrettyTable &insert_row(int row_index, const ArgType &arg,
                            const ArgsType &...args) {
      // check the num of the args
      if (sizeof...(args) + 1 != this->colms())
        throw std::runtime_error(
            "[ erro from 'PrettyTable::add_row' ] the number of items you want "
            "to add is not suitable for this table, it should be equal to the "
            "column count of this table");
      this->insert_args(0, row_index, arg, args...);
      ++this->_rows;
      return *this;
    }

#pragma endregion

#pragma region 'row' or 'column' counter
    /**
     * @brief get the row count of this table
     *
     * @return std::size_t
     */
    inline std::size_t rows() const { return this->_rows; }

    /**
     * @brief get the column count of this table
     *
     * @return std::size_t
     */
    inline std::size_t colms() const { return this->_tab.size(); }

#pragma endregion

#pragma region 'row' or 'column' deletor

    /**
     * @brief delete a row
     *
     * @param row_index the index of the row
     * @return true
     * @return false
     */
    inline bool del_row(int row_index) {
      // check the param 'row_index'
      if (row_index >= this->rows() || row_index < 0)
        return false;
      for (auto &elem : this->_tab) {
        elem.data().erase(elem.data().cbegin() + row_index);
        elem.refind_max_width();
      }
      --this->_rows;
      return true;
    }

    /**
     * @brief delete some rows
     *
     * @param start_row the start index of the row
     * @param n the count
     * @return true
     * @return false
     */
    bool del_rows(int start_row, std::size_t n) {
      // check the param 'start_row' and 'n'
      if (start_row >= this->rows() || start_row < 0 ||
          start_row + n > this->rows())
        return false;
      for (auto &elem : this->_tab) {
        elem.data().erase(elem.data().cbegin() + start_row,
                          elem.data().cbegin() + start_row + n);
        elem.refind_max_width();
      }

      this->_rows -= n;
      return true;
    }

    /**
     * @brief delete a column
     *
     * @param colm_index the index of the column
     * @return true
     * @return false
     */
    inline bool del_colm(int colm_index) {
      // check the param 'colm_index'
      if (colm_index >= this->colms() || colm_index < 0)
        return false;
      this->_tab.erase(this->_tab.cbegin() + colm_index);
      return true;
    }

    /**
     * @brief delete some columns
     *
     * @param start_colm the start index of the column
     * @param n the count
     * @return true
     * @return false
     */
    inline bool del_colms(int start_colm, std::size_t n) {
      // check the param'start_row' and 'n'
      if (start_colm >= this->colms() || start_colm < 0 ||
          start_colm + n > this->colms())
        return false;
      this->_tab.erase(this->_tab.cbegin() + start_colm,
                       this->_tab.cbegin() + start_colm + n);
      return true;
    }

    /**
     * @brief clear all rows but retain the header labels
     */
    inline void clear_rows() {
      for (auto &elem : this->_tab) {
        // update the max width
        elem.set_max_width(elem.header().size());
        elem.data().clear();
      }
      this->_rows = 0;
      return;
    }

    /**
     * @brief clear all rows includes the header labels
     */
    inline void clear() {
      this->_tab.clear();
      this->_rows = 0;
      return;
    }

#pragma endregion

#pragma region help methods
    /**
     * @brief trans the table to csv format
     *
     * @param splitor the splitor char
     * @return std::string
     */
    std::string to_csv(char splitor = ',') const {
      std::stringstream stream;
      stream << std::setiosflags(std::ios::fixed) << std::setprecision(this->_precision) << std::boolalpha;
      for (int j = 0; j != this->colms(); ++j) {
        stream << this->_tab.at(j).header();
        if (j != this->colms() - 1)
          stream << splitor;
      }
      stream << '\n';
      for (int i = 0; i != this->rows(); ++i) {
        for (int j = 0; j != this->colms(); ++j) {
          stream << this->get_item(i, j);
          if (j != this->colms() - 1)
            stream << splitor;
        }
        if (i != this->rows() - 1)
          stream << '\n';
      }
      return stream.str();
    }

    /**
     * @brief Get the elem in the table
     *
     * @param row_index the index of row
     * @param colm_index the index of column
     * @return const std::string&
     */
    inline const std::string &get_item(int row_index, int colm_index) const {
      return this->_tab.at(colm_index).data().at(row_index);
    }

    /**
     * @brief get the info of this table
     *
     * @return std::string
     */
    std::string table_info() const {
      std::stringstream stream;
      stream << "{'headers': [";
      for (int i = 0; i != this->colms(); ++i) {
        stream << this->_tab.at(i).header();
        if (i != this->colms() - 1)
          stream << ", ";
      }
      stream << "], 'align': " << this->get_align();
      stream << ", 'rows': " << this->rows();
      stream << ", 'colms': " << this->colms() << '}';
      return stream.str();
    }

#pragma endregion

  protected:
#pragma region protected methods
    /**
     * @brief insert the arguement(s) to the table
     *
     * @tparam ArgType the type of the arg
     * @tparam ArgsType the types of the args
     * @param colm_index the index of the column to insert
     * @param row_index the index of the column to insert
     * @param arg
     * @param args
     */
    template <typename ArgType, typename... ArgsType>
    void insert_args(std::size_t colm_index, std::size_t row_index,
                     const ArgType &arg, const ArgsType &...args) {
      // using stringstream to generate the string
      std::stringstream stream;
      stream << std::setiosflags(std::ios::fixed) << std::setprecision(this->_precision) << std::boolalpha;
      stream << arg;
      // get the string
      std::string str = stream.str();
      // insert the date
      auto &colm_data = this->_tab.at(colm_index).data();
      colm_data.insert(colm_data.cbegin() + row_index, str);
      // update the max width for the table column
      if (str.size() > this->_tab.at(colm_index).get_max_width())
        this->_tab.at(colm_index).set_max_width(str.size());
      return insert_args(colm_index + 1, row_index, args...);
    }

    /**
     * @brief insert the arguement(s) to the table
     *
     * @tparam ArgType the type of the arg
     * @tparam ArgsType the types of the args
     * @param colm_index the index of the column to insert
     * @param row_index the index of the column to insert
     * @param arg
     */
    template <typename ArgType>
    void insert_args(std::size_t colm_index, std::size_t row_index,
                     const ArgType &arg) {
      // using stringstream to generate the string
      std::stringstream stream;
      stream << std::setiosflags(std::ios::fixed) << std::setprecision(this->_precision) << std::boolalpha;
      stream << arg;
      // get the string
      std::string str = stream.str();
      // insert the date
      auto &colm_data = this->_tab.at(colm_index).data();
      colm_data.insert(colm_data.cbegin() + row_index, str);
      // update the max width for the table column
      if (str.size() > this->_tab.at(colm_index).get_max_width())
        this->_tab.at(colm_index).set_max_width(str.size());
      return;
    }

#pragma endregion
  };

  namespace ns_priv {
    class Printer {
    public:
      Printer() = default;

      std::ostream &operator()(std::ostream &os, const PrettyTable &pretab) {
        if (pretab.get_table().empty()) {
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
        for (int i = 0; i != pretab.colms(); ++i) {
          stream << std::string(pretab.get_table().at(i).get_max_width() + 2, '-') << '+';
        }
        return stream.str();
      }

      std::ostream &print_head(std::ostream &os, const PrettyTable &pretab) {
        os << '|';
        for (int i = 0; i != pretab.colms(); ++i) {
          auto &colm = pretab.get_table().at(i);
          this->print_item(os, colm.header(), colm.get_max_width(), TabAlign::CENTER) << '|';
        }
        return os;
      }

      std::ostream &print_data(std::ostream &os, const PrettyTable &pretab, const std::string &line) {
        for (int j = 0; j != pretab.rows(); ++j) {
          os << '|';
          for (int i = 0; i != pretab.colms(); ++i) {
            auto &colm = pretab.get_table().at(i);
            this->print_item(os, colm.data().at(j), colm.get_max_width(), pretab.get_align()) << '|';
          }
          os << '\n'
             << line << '\n';
        }
        return os;
      }

      std::ostream &print_item(std::ostream &os, const std::string &content, int colm_width, TabAlign align) {
        os << ' ';
        switch (align) {
        case TabAlign::CENTER: {
          int left_space = (colm_width - content.size()) / 2;
          int right_space = colm_width - left_space - content.size();
          os << std::string(left_space, ' ') << content << std::string(right_space, ' ');
        } break;
        case TabAlign::LEFT: {
          int left_space = (colm_width - content.size()) / 2;
          int right_space = colm_width - left_space - content.size();
          os << std::left << std::setw(colm_width) << content;
        } break;
        case TabAlign::RIGHT: {
          os << std::right << std::setw(colm_width) << content;
        } break;
        }
        os << ' ';
        return os;
      }
    } printer;
  } // namespace ns_priv

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

} // namespace ns_pretab
