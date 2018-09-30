#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "umaumaxcpp/streampp.hpp"

int main() {
  std::ostringstream oss;
  {
    boost::archive::text_oarchive ot(oss);

    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ot << v;

    std::map<std::string, int> data;
    data["homu"] = 13;
    data["mami"] = 15;
    data["mado"] = 14;
    data["saya"] = 13;
    data["an"]   = 11;
    ot << data;
  }

  std::cout << oss.str();

  std::istringstream iss(oss.str());
  {
    boost::archive::text_iarchive it(iss);
    std::vector<int> v;
    it >> v;
    std::map<std::string, int> data;
    it >> data;
    std::cout << data;
  }
  return 0;
}
