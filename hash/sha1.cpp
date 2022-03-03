#include <iostream>
#include <string>

#include <boost/version.hpp>
#if BOOST_VERSION < 106800
#include <boost/uuid/sha1.hpp>
#else
#include <boost/uuid/detail/sha1.hpp>
#endif
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

std::string sha1(const std::string& str) {
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(str.data(), str.size());
  constexpr int sha1_elem_num = 5;
  unsigned int digest_buf[sha1_elem_num];
  boost::uuids::detail::sha1::digest_type& digest = digest_buf;
  sha1.get_digest(digest);
  std::string result;
  boost::algorithm::hex(digest, digest + sha1_elem_num,
                        std::back_inserter(result));
  boost::algorithm::to_lower(result);
  return result;
}

int main(int argc, char* argv[]) {
  std::cout << sha1("Hello world!") << std::endl;
  return 0;
}
