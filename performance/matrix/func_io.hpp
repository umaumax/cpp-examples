#ifndef FUNC_IO_HPP_INCLUDED__
#define FUNC_IO_HPP_INCLUDED__

#include <boost/archive/basic_archive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/binary_object.hpp>
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

void example_func() {
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

#endif  // FUNC_IO_HPP_INCLUDED__

