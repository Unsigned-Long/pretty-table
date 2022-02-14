#include <iostream>

#include "artwork/geometry/point.hpp"
#include "prettytable.hpp"

int main(int argc, char const* argv[]) {
  try {
    ns_pretab::PrettyTable tab(ns_pretab::TabAlign::CENTER);
    std::cout << tab << std::endl;

    tab.append_colums({"id", "z"});
    tab.append_row(12, 34.5f);

    // tab.add_row(12, 34.5f, 12.3f, 45.6f);
    /**
     * @brief [ erro from 'PrettyTable::add_row' ] the number of items you want
     * to add is not suitable for this table, it should be equal to the column
     * count of this table
     */

    std::cout << tab << std::endl;

    tab.insert_colums({"x", "y"}, 1);
    std::cout << tab << std::endl;

    ns_geo::RefPoint3f p(0, 12.0f, 45.0f, 67.0f);
    tab.append_row(p.id(), p.x(), p.y(), p.z());
    std::cout << tab << std::endl;

    auto rps = ns_geo::RefPointSet3f::randomGenerator(5, 0.0f, 1.0f, 0.0f, 1.0f,
                                                      0.0f, 1.0f);
    for (const auto& [id, p] : rps) {
      tab.append_row(p.id(), p.x(), p.y(), p.z());
    }
    std::cout << tab << std::endl;

    tab.del_row(0);
    std::cout << tab << std::endl;

    tab.del_colm(0);
    tab.set_align(ns_pretab::TabAlign::RIGHT);
    std::cout << tab << std::endl;

    std::cout << tab.to_csv() << std::endl;

    std::cout << tab.table_info() << std::endl;

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
