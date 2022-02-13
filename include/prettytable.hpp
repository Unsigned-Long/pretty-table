#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ns_pretab {

struct TableColumn {
 private:
  /**
   * @brief the members
   */
  std::string _header;
  std::vector<std::string> _data;
  std::size_t _max_szie;

 public:
  /**
   * @brief construct a new TableColumn object
   */
  TableColumn() = default;

  TableColumn(const std::string &header, const std::vector<std::string> &data)
      : _header(header), _data(data), _max_szie(header.size()) {}

  inline std::string &header() { return this->_header; }
  inline const std::string &header() const { return this->_header; }

  inline std::vector<std::string> &data() { return this->_data; }
  inline const std::vector<std::string> &data() const { return this->_data; }

  inline std::size_t size() const { return this->_data.size(); }

  inline std::size_t elem_max_size() const { return this->_max_szie; }

  inline void set_max_size(std::size_t max_size) { this->_max_szie = max_size; }

  inline void refind_max_size() {
    this->_max_szie = this->_header.size();
    for (const auto &elem : this->_data)
      if (this->_max_szie < elem.size()) this->_max_szie = elem.size();
    return;
  }
};

class PrettyTable {
 private:
  std::vector<TableColumn> _tab;

  std::size_t _precision;

  std::size_t _rows;

 public:
#pragma region constructors
  /**
   * @brief Construct a new PrettyTable object
   *
   * @param precision the precision
   */
  PrettyTable(std::size_t precision = 3) : _precision(precision), _rows(0) {}

  /**
   * @brief Construct a new PrettyTable object
   *
   * @param headers the header labels
   * @param precision the precision
   */
  PrettyTable(const std::vector<std::string> &headers,
              std::size_t precision = 3)
      : _precision(precision), _rows(0) {
    this->_tab.resize(headers.size());
    for (int i = 0; i != headers.size(); ++i) {
      this->_tab.at(i).header() = headers.at(i);
      this->_tab.at(i).set_max_size(headers.at(i).size());
    }
  }

#pragma endregion

#pragma region 'set' methods
  /**
   * @brief Set the precision for float value
   *
   * @param precision the precision
   */
  inline void set_precision(std::size_t precision) {
    this->_precision = precision;
    return;
  }
#pragma endregion

#pragma region add 'row' or 'column'

  /**
   * @brief add a column at the end of this table
   *
   * @param header the header label of this column
   * @return PrettyTable&
   */
  inline PrettyTable &add_colum(const std::string &header) {
    this->add_colum_at(header, this->colms());
    return *this;
  }

  /**
   * @brief add a column at the 'colm_index' of this table
   *
   * @param header the header table
   * @param colm_index the index of the column
   * @return PrettyTable&
   */
  inline PrettyTable &add_colum_at(const std::string &header, int colm_index) {
    auto new_colm_data = std::vector<std::string>(this->rows());
    if (colm_index < 0 || colm_index >= this->_tab.size())
      this->_tab.push_back(TableColumn(header, new_colm_data));
    else
      this->_tab.insert(this->_tab.cbegin() + colm_index,
                        TableColumn(header, new_colm_data));
    return *this;
  }

  /**
   * @brief add some columns at the end of this table
   *
   * @param headers
   * @return PrettyTable&
   */
  inline PrettyTable &add_colums(const std::vector<std::string> &headers) {
    this->add_colums_at(headers, this->colms());
    return *this;
  }

  /**
   * @brief add a column at the 'colm_index' of this table
   *
   * @param headers the header labels
   * @param colm_index the index of the column to start inserting
   * @return PrettyTable&
   */
  inline PrettyTable &add_colums_at(const std::vector<std::string> &headers,
                                    int colm_index) {
    std::vector<TableColumn> new_colms(headers.size());
    for (int i = 0; i != headers.size(); ++i) {
      new_colms.at(i).header() = headers.at(i);
      new_colms.at(i).set_max_size(headers.at(i).size());
      new_colms.at(i).data().resize(this->rows());
    }
    if (colm_index < 0 || colm_index >= this->_tab.size())
      this->_tab.insert(this->_tab.cend(), new_colms.cbegin(),
                        new_colms.cend());
    else
      this->_tab.insert(this->_tab.cbegin() + colm_index, new_colms.cbegin(),
                        new_colms.cend());
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
  inline PrettyTable &add_row(const ArgType &arg, const ArgsType &...args) {
    this->add_row_at(this->rows(), arg, args...);
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
  inline PrettyTable &add_row_at(int row_index, const ArgType &arg,
                                 const ArgsType &...args) {
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

#pragma region 'row count' or 'column count' or 'table'
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

  /**
   * @brief get the table
   *
   * @return const std::vector<TableColumn>&
   */
  inline const std::vector<TableColumn> &table() const { return this->_tab; }

#pragma endregion

#pragma region 'delete' or 'clear'

  /**
   * @brief delete a row
   *
   * @param row_index the index of the row
   * @return true
   * @return false
   */
  inline bool del_row(int row_index) {
    if (row_index >= this->rows() || row_index < 0) return false;
    for (auto &elem : this->_tab) {
      elem.data().erase(elem.data().cbegin() + row_index);
      elem.refind_max_size();
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
  inline bool del_rows(int start_row, std::size_t n) {
    if (start_row >= this->rows() || start_row < 0 ||
        start_row + n > this->rows())
      return false;
    for (auto &elem : this->_tab) {
      elem.data().erase(elem.data().cbegin() + start_row,
                        elem.data().cbegin() + start_row + n);
      elem.refind_max_size();
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
    if (colm_index >= this->colms() || colm_index < 0) return false;
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
      elem.set_max_size(elem.header().size());
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
  inline std::string to_csv(char splitor = ',') const {
    std::stringstream stream;
    for (int j = 0; j != this->colms(); ++j) {
      stream << this->_tab.at(j).header();
      if (j != this->colms() - 1) stream << splitor;
    }
    stream << '\n';
    for (int i = 0; i != this->rows(); ++i) {
      for (int j = 0; j != this->colms(); ++j) {
        stream << this->get_elem(i, j);
        if (j != this->colms() - 1) stream << splitor;
      }
      if (i != this->rows() - 1) stream << '\n';
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
  inline const std::string &get_elem(int row_index, int colm_index) const {
    return this->_tab.at(colm_index).data().at(row_index);
  }
#pragma endregion

#pragma region protected methods
 protected:
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
    std::stringstream stream;
    stream << std::setiosflags(std::ios::fixed)
           << std::setprecision(this->_precision);
    stream << arg;
    std::string str;
    stream >> str;
    auto &colm_data = this->_tab.at(colm_index).data();
    colm_data.insert(colm_data.cbegin() + row_index, str);
    if (str.size() > this->_tab.at(colm_index).elem_max_size())
      this->_tab.at(colm_index).set_max_size(str.size());
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
    std::stringstream stream;
    stream << std::setiosflags(std::ios::fixed)
           << std::setprecision(this->_precision);
    stream << arg;
    std::string str;
    stream >> str;
    auto &colm_data = this->_tab.at(colm_index).data();
    colm_data.insert(colm_data.cbegin() + row_index, str);
    if (str.size() > this->_tab.at(colm_index).elem_max_size())
      this->_tab.at(colm_index).set_max_size(str.size());
    return;
  }

#pragma endregion
};

/**
 * @brief output format for the pretty table
 *
 * @param os the ostream
 * @param pretab the pretty table
 * @return std::ostream&
 */
static std::ostream &operator<<(std::ostream &os, const PrettyTable &pretab) {
  if (pretab.table().empty()) {
    os << PrettyTable({"empty"});
    return os;
  }
  // line
  std::string line("+");
  for (const auto &elem : pretab.table())
    line += std::string(elem.elem_max_size() + 2, '-') + '+';
  os << line << std::endl;
  // print headers
  os << '|';
  for (const auto &elem : pretab.table()) {
    auto left_space = static_cast<std::size_t>(
        (elem.elem_max_size() + 2 - elem.header().size()) / 2);
    auto right_space =
        elem.elem_max_size() + 2 - elem.header().size() - left_space;
    os << std::string(left_space, ' ') << elem.header()
       << std::string(right_space, ' ') << '|';
  }
  os << '\n' << line;
  // print data
  for (int i = 0; i != pretab.rows(); ++i) {
    os << "\n|";
    for (int j = 0; j != pretab.colms(); ++j) {
      const auto &colm = pretab.table().at(j);
      const auto &elem = pretab.table().at(j).data().at(i);
      auto left_space = static_cast<std::size_t>(
          (colm.elem_max_size() + 2 - elem.size()) / 2);
      auto right_space = colm.elem_max_size() + 2 - elem.size() - left_space;
      os << std::string(left_space, ' ') << elem
         << std::string(right_space, ' ') << '|';
    }
    os << '\n' << line;
  }
  return os;
}

}  // namespace ns_pretab
