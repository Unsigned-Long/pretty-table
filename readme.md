# Pretty Table

[TOC]

___Aurther: csl___

___E-Mainl: 3079625093@qq.com___

```cpp
                                _|      _|                
_|_|_|    _|  _|_|    _|_|    _|_|_|_|_|_|_|_|  _|    _|  
_|    _|  _|_|      _|_|_|_|    _|      _|      _|    _|  
_|    _|  _|        _|          _|      _|      _|    _|  
_|_|_|    _|          _|_|_|      _|_|    _|_|    _|_|_|  
_|                                                    _|  
_|                                                _|_|    
                                            
  _|                _|        _|            
_|_|_|_|    _|_|_|  _|_|_|    _|    _|_|    
  _|      _|    _|  _|    _|  _|  _|_|_|_|  
  _|      _|    _|  _|    _|  _|  _|        
    _|_|    _|_|_|  _|_|_|    _|    _|_|_|  
```



## OverView

this lib is used to tabulate data on the console:

```cpp
// cpp source code
std::cout << ns_pretab::PrettyTable({"lib-name", "version"}).add_row("Pretty Table", "0.0.1") << std::endl;
// output
+----------+---------+
| lib-name | version |
+----------+---------+
|  Pretty  |  0.0.1  |
+----------+---------+
```



## Usage

### Source Code

```cpp
#include <iostream>

#include "prettytable.hpp"

int main(int argc, char const* argv[]) {
  try {
    ns_pretab::PrettyTable tab(ns_pretab::TabAlign::CENTER);
    // output[1]
    std::cout << tab << std::endl;

    tab.add_colums({"id", "z"});
    tab.add_row(12, 34.5f);

    // output[2]
    std::cout << tab << std::endl;

    tab.add_colums_at({"x", "y"}, 1);
    // output[3]
    std::cout << tab << std::endl;

    ns_geo::RefPoint3f p(0, 12.0f, 45.0f, 67.0f);
    tab.add_row(p.id(), p.x(), p.y(), p.z());
    // output[4]
    std::cout << tab << std::endl;

    auto rps = ns_geo::RefPointSet3f::randomGenerator(5, 0.0f, 1.0f, 0.0f, 1.0f,
                                                      0.0f, 1.0f);
    for (const auto& [id, p] : rps) {
      tab.add_row(p.id(), p.x(), p.y(), p.z());
    }
    // output[5]
    std::cout << tab << std::endl;

    tab.del_row(0);
    // output[6]
    std::cout << tab << std::endl;

    tab.del_colm(0);
    tab.set_align(ns_pretab::TabAlign::RIGHT);
    // output[7]
    std::cout << tab << std::endl;
      
    // output[8]
    std::cout << tab.to_csv() << std::endl;

    // output[9]
    std::cout << tab.table_info() << std::endl;

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
```

### Output

output[1]

```cpp
+-------+
| empty |
+-------+
```

output[2]

```cpp
+----+--------+
| id |   z    |
+----+--------+
| 12 | 34.500 |
+----+--------+
```

output[3]

```cpp
+----+---+---+--------+
| id | x | y |   z    |
+----+---+---+--------+
| 12 |   |   | 34.500 |
+----+---+---+--------+
```

output[4]

```cpp
+----+--------+--------+--------+
| id |   x    |   y    |   z    |
+----+--------+--------+--------+
| 12 |        |        | 34.500 |
+----+--------+--------+--------+
| 0  | 12.000 | 45.000 | 67.000 |
+----+--------+--------+--------+
```

output[5]

```cpp
+----+--------+--------+--------+
| id |   x    |   y    |   z    |
+----+--------+--------+--------+
| 12 |        |        | 34.500 |
+----+--------+--------+--------+
| 0  | 12.000 | 45.000 | 67.000 |
+----+--------+--------+--------+
| 4  | 0.701  | 0.654  | 0.527  |
+----+--------+--------+--------+
| 3  | 0.930  | 0.687  | 0.067  |
+----+--------+--------+--------+
| 2  | 0.008  | 0.530  | 0.035  |
+----+--------+--------+--------+
| 1  | 0.519  | 0.935  | 0.679  |
+----+--------+--------+--------+
| 0  | 0.219  | 0.459  | 0.132  |
+----+--------+--------+--------+
```

output[6]

```cpp
+----+--------+--------+--------+
| id |   x    |   y    |   z    |
+----+--------+--------+--------+
| 0  | 12.000 | 45.000 | 67.000 |
+----+--------+--------+--------+
| 4  | 0.701  | 0.654  | 0.527  |
+----+--------+--------+--------+
| 3  | 0.930  | 0.687  | 0.067  |
+----+--------+--------+--------+
| 2  | 0.008  | 0.530  | 0.035  |
+----+--------+--------+--------+
| 1  | 0.519  | 0.935  | 0.679  |
+----+--------+--------+--------+
| 0  | 0.219  | 0.459  | 0.132  |
+----+--------+--------+--------+
```

output[7]

```cpp
+--------+--------+--------+
|      x |      y |      z |
+--------+--------+--------+
| 12.000 | 45.000 | 67.000 |
+--------+--------+--------+
|  0.701 |  0.654 |  0.527 |
+--------+--------+--------+
|  0.930 |  0.687 |  0.067 |
+--------+--------+--------+
|  0.008 |  0.530 |  0.035 |
+--------+--------+--------+
|  0.519 |  0.935 |  0.679 |
+--------+--------+--------+
|  0.219 |  0.459 |  0.132 |
+--------+--------+--------+
```

output[8]

```cpp
x,y,z
12.000,45.000,67.000
0.701,0.654,0.527
0.930,0.687,0.067
0.008,0.530,0.035
0.519,0.935,0.679
0.219,0.459,0.132
```

output[9]

```cpp
{'headers': [x, y, z], 'align': RIGHT, 'rows': 6, 'colms': 3}
```

## Apis

### TabAlign

```cpp
enum class TabAlign {
  /**
   * @brief options
   */
  RIGHT,
  LEFT,
  CENTER
};
```

### Constructors

+ ___PrettyTable(TabAlign align = TabAlign::CENTER, std::size_t precision = 3)___

```cpp
  /**
   * @brief Construct a new PrettyTable object
   * 
   * @param align the align
   * @param precision the precision
   */
```

+ ___PrettyTable(const std::vector< std::string> &headers, TabAlign align = TabAlign::CENTER, std::size_t precision = 3)___

```cpp
  /**
   * @brief Construct a new PrettyTable object
   *
   * @param headers the header labels
   * @param align the align
   * @param precision the precision
   */
```

### 'set' methods

+ ___inline void set_precision(std::size_t precision)___

```cpp
  /**
   * @brief Set the precision for float value
   *
   * @param precision the precision
   */
```

+ ___inline void set_align(TabAlign align)___

```cpp
  /**
   * @brief Set the align 
   * 
   * @param align the align
   */
```

### 'row' or 'column' methods

+ ___inline PrettyTable &add_colum(const std::string &header)___

```cpp
  /**
   * @brief add a column at the end of this table
   *
   * @param header the header label of this column
   * @return PrettyTable&
   */
```

+ ___inline PrettyTable &add_colum_at(const std::string &header, int colm_index)___

```cpp
  /**
   * @brief add a column at the 'colm_index' of this table
   *
   * @param header the header table
   * @param colm_index the index of the column
   * @return PrettyTable&
   */
```

+ ___inline PrettyTable &add_colums(const std::vector< std::string> &headers)___

```cpp
  /**
   * @brief add some columns at the end of this table
   *
   * @param headers
   * @return PrettyTable&
   */
```

+ ___inline PrettyTable &add_colums_at(const std::vector< std::string> &headers, int colm_index)___

```cpp
  /**
   * @brief add a column at the 'colm_index' of this table
   *
   * @param headers the header labels
   * @param colm_index the index of the column to start inserting
   * @return PrettyTable&
   */
```

+ ___template <typename ArgType, typename... ArgsType> inline PrettyTable &add_row(const ArgType &arg, const ArgsType &...args)___

```cpp
  /**
   * @brief add a row to this table at the end
   *
   * @tparam ArgType the type of the arg
   * @tparam ArgsType the types of the args
   * @param arg
   * @param args
   * @return PrettyTable&
   */
```

+ ___template <typename ArgType, typename... ArgsType> inline PrettyTable &add_row_at(int row_index, const ArgType &arg, const ArgsType &...args)___

```cpp
  /**
   * @brief add a row to this table at the 'row_index'
   *
   * @tparam ArgType the type of the arg
   * @tparam ArgsType the types of the args
   * @param arg
   * @param args
   * @return PrettyTable&
   */
```

### 'row count' or 'column count' or 'table'  methods

+ ___inline std::size_t rows() const___

```cpp
  /**
   * @brief get the row count of this table
   *
   * @return std::size_t
   */
```

+ ___inline std::size_t colms() const___

```cpp
  /**
   * @brief get the column count of this table
   *
   * @return std::size_t
   */
```

+ ___inline const std::vector<TableColumn> &table() const___

```cpp
  /**
   * @brief get the table
   *
   * @return const std::vector<TableColumn>&
   */
```

### 'delete' or 'clear'  methods

+ ___inline bool del_row(int row_index)___

```cpp
  /**
   * @brief delete a row
   *
   * @param row_index the index of the row
   * @return true
   * @return false
   */
```

+ ___inline bool del_rows(int start_row, std::size_t n)___

```cpp
  /**
   * @brief delete some rows
   *
   * @param start_row the start index of the row
   * @param n the count
   * @return true
   * @return false
   */
```

+ ___inline bool del_colm(int colm_index)___

```cpp
  /**
   * @brief delete a column
   *
   * @param colm_index the index of the column
   * @return true
   * @return false
   */
```

+ ___inline bool del_colms(int start_colm, std::size_t n)___

```cpp
  /**
   * @brief delete some columns
   *
   * @param start_colm the start index of the column
   * @param n the count
   * @return true
   * @return false
   */
```

+ ___inline void clear_rows()___

```cpp
  /**
   * @brief clear all rows but retain the header labels
   */
```

+ ___inline void clear()___

```cpp
  /**
   * @brief clear all rows includes the header labels
   */
```

### 'help' methods

+ ___inline std::string to_csv(char splitor = ',') const___

```cpp
  /**
   * @brief trans the table to csv format
   *
   * @param splitor the splitor char
   * @return std::string
   */
```

+ ___inline const std::string &get_elem(int row_index, int colm_index) const___

```cpp
  /**
   * @brief Get the elem in the table
   *
   * @param row_index the index of row
   * @param colm_index the index of column
   * @return const std::string&
   */
```

+ ___inline std::string table_info() const___

```cpp
  /**
   * @brief get the info of this table
   *
   * @return std::string
   */
```

