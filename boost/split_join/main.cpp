#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  std::vector<std::string> v;

  // NOTE: split
  {
    std::string str   = "  Nanoha, Fate, Hayate  ";
    std::string delim = ", ";
    // NOTE: 最初と最後の余計なdelimを除去(第一引数の変数が上書きされる)
    boost::trim_if(str, boost::is_any_of(delim));
    // NOTE: you can use boost::is_space() as delim
    // NOTE: boost::algorithm::token_compress_on: 連続したdelimを1つのdelimとして扱う
    boost::algorithm::split(v, str, boost::is_any_of(delim), boost::algorithm::token_compress_on);
  }

  // NOTE: join
  {
    std::string delim      = " - ";
    std::string joined_str = boost::algorithm::join(v, delim);
    std::cout << joined_str << std::endl;
  }
  return 0;
}
