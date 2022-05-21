#include "artwork/csv/csv.h"
#include "artwork/timer/timer.h"
#include "include/prettytable.hpp"

struct Info {
private:
  /**
   * @brief the members
   */
  int _id;
  std::string _name;
  float _score;

public:
  /**
   * @brief construct a new Info object
   */
  Info(const int &id, const std::string &name, const float &score)
      : _id(id), _name(name), _score(score) {}

  inline int &id() { return this->_id; }
  inline const int &id() const { return this->_id; }

  inline std::string &name() { return this->_name; }
  inline const std::string &name() const { return this->_name; }

  inline float &score() { return this->_score; }
  inline const float &score() const { return this->_score; }
};

void example0() {
  ns_pretab::PrettyTable tab(1);

  tab.addGrid(0, 0, "libname").addGrid(0, 1, "github");
  tab.addGrid(1, 0, "pretty-table").addGrid(1, 1, "https://github.com/Unsigned-Long/pretty-table.git");

  std::cout << tab << std::endl;
}

void example1() {
  ns_pretab::PrettyTable tab(1);

  tab.addGrid(1, 1, "hello", ns_pretab::Align::CENTER, 2, 2);
  tab.addGrid(0, 4, "world", ns_pretab::Align::CENTER, 2, 2);
  tab.addGrid(2, 3, "pretty-table", ns_pretab::Align::CENTER, 1, 3);
  tab.addGrid(0, 3, "Ubuntu", ns_pretab::Align::CENTER, 2, 1);
  tab.addGrid(0, 0, "cpp", ns_pretab::Align::CENTER, 1, 2);
  tab.addGrid(1, 0, "SGG", ns_pretab::Align::CENTER, 2, 1);
  tab.addGrid(0, 2, "^-^", ns_pretab::Align::CENTER, 1, 1);

  std::cout << tab << std::endl;
}

void example2() {
  ns_pretab::PrettyTable tab(1);

  auto [h, vec] = CSV_READ_FILE_H("../data/info.csv", ',', Info, int, std::string, float);
  tab.addGrid(0, 0, h.at(0)).addGrid(0, 1, h.at(1)).addGrid(0, 2, h.at(2));
  for (int i = 0; i != vec.size(); i++) {
    auto &elem = vec.at(i);
    tab.addGrid(i + 1, 0, elem.id()).addGrid(i + 1, 1, elem.name()).addGrid(i + 1, 2, elem.score());
  }

  std::cout << tab << std::endl;
}

int main(int argc, char const *argv[]) {
  try {
    example0();
    example1();
    example2();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}