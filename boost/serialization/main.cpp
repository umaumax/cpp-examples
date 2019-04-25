#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include "cmdline.h"
#include "umaumaxcpp/streampp.hpp"

struct Person {
  int age;
  std::string name;
  std::vector<int> items;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, unsigned int version) {
    ar& age;
    ar& name;
    ar& items;
  }
};

std::ostream& operator<<(std::ostream& os, const Person& v) {
  os << "name:" << v.name << ", age: " << v.age;
  os << ", items: [";
  for (auto& v : v.items) {
    os << v << ", ";
  }
  os << "]";
  return os;
}

int main(int argc, const char* argv[]) {
  cmdline::parser a;
#define EXAMPLES "basic", "stringstream", "fstream", "struct", "gzip"
  std::vector<std::string> examples{EXAMPLES};
  auto help =
      std::string("example pattern:") + boost::algorithm::join(examples, ", ");
  a.add<std::string>("example", 'e', help, false, examples[0],
                     cmdline::oneof<std::string>(EXAMPLES));
  a.parse_check(argc, const_cast<char**>(argv));

  auto example = a.get<std::string>("example");
  if (example == "basic") {
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
      std::cout << data << std::endl;

      std::streampos archive_offset = iss.tellg();
      std::streamoff stream_end_offset =
          iss.seekg(0, std::ios_base::end).tellg();
      iss.seekg(archive_offset);
      if (iss.tellg() == stream_end_offset - 1) {
        std::cout << "EOF" << std::endl;
      }
    }
  }

  if (example == "stringstream") {
    std::ostringstream oss;
    {
      boost::archive::text_oarchive toa(oss);
      // NOTE: stringstreamの場合には自動的に改行される
      toa << std::string("hello");
    }
    {
      boost::archive::text_oarchive toa(oss);
      toa << std::string("hello");
    }
    std::cout << oss.str();

    std::istringstream iss(oss.str());
    {
      boost::archive::text_iarchive tia(iss);
      std::string msg;
      tia >> msg;
      std::cout << msg;
    }
    {
      boost::archive::text_iarchive tia(iss);
      std::string msg;
      tia >> msg;
      std::cout << msg;
    }
  }

  if (example == "fstream") {
    // NOTE: multi data to one file
    auto filename = ".boost_serialization_tmp";
    // NOTE: truncate
    {
      std::fstream ofs(
          filename, std::fstream::out | std::ios::binary | std::fstream::trunc);
      std::stringstream oss;
      boost::archive::text_oarchive toa(oss);
      toa << std::string("hello");
      // NOTE: stringstreamを仲介する場合にはstd::endlが必須
      // NOTE: 直接 boost::archive::text_oarchive toa(ofs); とする場合には不要
      ofs << oss.str() << std::endl;
    }
    // NOTE: append
    {
      std::fstream ofs(
          filename, std::fstream::out | std::ios::binary | std::fstream::app);
      std::stringstream oss;
      boost::archive::text_oarchive toa(oss);
      toa << std::string("hello");
      ofs << oss.str() << std::endl;
    }
    // NOTE: loop
    {
      std::ifstream ifs(filename);
      std::streampos archive_offset = ifs.tellg();
      std::streamoff stream_end_offset =
          ifs.seekg(0, std::ios_base::end).tellg();
      ifs.seekg(archive_offset);
      while (true) {
        if (ifs.tellg() == stream_end_offset - 1) break;
        boost::archive::text_iarchive tia(ifs);
        std::string msg;
        tia >> msg;
        std::cout << msg;
      }
    }
  }

  if (example == "gzip") {
    std::stringstream compressed;
    // NOTE: compress
    {
      std::stringstream ss;
      {
        boost::archive::text_oarchive toa(ss);
        toa << std::string("hello");
      }

      boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
      out.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(
          boost::iostreams::gzip::best_compression)));
      out.push(ss);
      boost::iostreams::copy(out, compressed);

      std::cout << compressed.str() << std::endl;
    }
    // NOTE: decompress
    {
      boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
      out.push(boost::iostreams::gzip_decompressor());
      out.push(compressed);
      std::stringstream decompressed;
      boost::iostreams::copy(out, decompressed);

      {
        boost::archive::text_iarchive tia(decompressed);
        std::string msg;
        tia >> msg;
        std::cout << msg;
      }
    }
  }

  if (example == "struct") {
    std::ostringstream oss;
    {
      boost::archive::text_oarchive toa(oss);
      Person person;
      person.name  = "name";
      person.age   = 17;
      person.items = std::vector<int>{0, 1, 2, 3, 4};
      toa << person;
    }
    std::cout << "person archive output:" << std::endl;
    std::cout << oss.str();

    std::istringstream iss(oss.str());
    {
      boost::archive::text_iarchive tia(iss);
      Person person;
      tia >> person;

      std::cout << "person output from archive:" << std::endl;
      std::cout << person << std::endl;
    }
  }
  return 0;
}
