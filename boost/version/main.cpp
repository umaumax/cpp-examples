#include <iostream>

#include <boost/version.hpp>

int main() {
  // NOTE: e.g. 106900
  std::cout << BOOST_VERSION << std::endl;
  // NOTE: e.g.  1_69
  std::cout << BOOST_LIB_VERSION << std::endl;
}
