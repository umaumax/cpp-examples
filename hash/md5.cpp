#include <iostream>
#include <string>

#include <boost/version.hpp>
#if BOOST_VERSION < 106800
#include <boost/uuid/md5.hpp>
#else
#include <boost/uuid/detail/md5.hpp>
#endif
#include <boost/algorithm/hex.hpp>

std::string md5(const std::string& str) {
  boost::uuids::detail::md5 md5;
  boost::uuids::detail::md5::digest_type digest;
  md5.process_bytes(str.data(), str.size());
  md5.get_digest(digest);
  const auto char_digest = reinterpret_cast<const char*>(&digest);
  std::string result;
  boost::algorithm::hex(
      char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type),
      std::back_inserter(result));
  return result;
}

int main(int argc, char* argv[]) {
  std::cout << md5("Hello world!") << std::endl;
  return 0;
}
