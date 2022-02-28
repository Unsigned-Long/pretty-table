#include "artwork/geometry/point.hpp"
#include "prettytable.hpp"

int main(int argc, char const *argv[]) {
  try {
    // create a table from a csv file 
    auto tab = ns_pretab::PrettyTable::fromCSV("../data/info.csv", 3, 3);

    // set aligns foe columns
    tab.setAlign(1, ns_pretab::TabAlign::LEFT);
    tab.setAlign(2, ns_pretab::TabAlign::RIGHT);

    // output the table info
    std::cout << tab.tableInfo() << std::endl;
    // print the table
    std::cout << tab << std::endl;

    // define a table
    ns_pretab::PrettyTable tab2({"id", "X", "Y", "Z"});
    // some settings
    tab2.setAlign(ns_pretab::TabAlign::RIGHT).setPrecision(3);

    // add data
    auto rps = ns_geo::RefPointSet3f::randomGenerator(9, 0.0f, 10.0f, 10.0f, 434.0f, 23.0f, 156.0f);
    for (const auto &[id, rp] : rps) {
      tab2.appendRow(id, rp.x(), rp.y(), rp.z());
    }

    //add a new column
    tab2.appendColumn("distance", ns_pretab::TabAlign::RIGHT, 3);
    for (int i = 0; i != rps.size(); ++i) {
      tab2.setItem(ns_geo::distance(ns_geo::Point3f(0.0f, 0.0f, 0.0f), rps.at(i)), i, 4);
    }

    // some print work
    std::cout << tab2.tableInfo() << std::endl;
    std::cout << tab2 << std::endl;

    // trans
    std::cout << tab2.toCSV() << std::endl;
    std::cout << tab2.toMarkDown() << std::endl;

  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
