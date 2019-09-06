#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "lz4.h"

// TODO: delete debug codes
#define VAR_DEBUG(x) std::cout << "[VAR_DEBUG]:" #x << "=" << x << std::endl;

int main(int argc, char* argv[]) {
  bool fast_flag = false;
  std::string sz_source_str =
      "I am the bone of my sword."
      "Steel is my body,and fire is my blood."
      "I have created over a thousand blades."
      "Unknown to Death."
      "Nor known to Life."
      "Have withstood pain to create many weapons."
      "Yet,those hands will never hold anything."
      "So as I pray,unlimited blade works.";
  int input_size        = sz_source_str.size();
  const char* sz_source = sz_source_str.c_str();

  // ---- ---- ---- ----
  // ---- ---- ---- ----
  // compress
  // ---- ---- ---- ----
  // ---- ---- ---- ----
  int worst_compressed_size = LZ4_compressBound(input_size);
  VAR_DEBUG(worst_compressed_size);
  int dst_capacity = worst_compressed_size;
  std::vector<char> compressed_vec(dst_capacity);
  char* compressed = compressed_vec.data();
  int compressed_size;
  compressed_size =
      LZ4_compress_default(sz_source, compressed, input_size, dst_capacity);
  if (fast_flag) {
    int acceleration = 4;
    compressed_size  = LZ4_compress_fast(sz_source, compressed, input_size,
                                        dst_capacity, acceleration);
  }
  if (compressed_size == 0) {
    std::cerr << "lz4 compress failed because of few dst buffer size"
              << std::endl;
    return 1;
  }
  // NOTE: shrink beacuse new size <= old size
  compressed_vec.resize(compressed_size);

  VAR_DEBUG(input_size);
  VAR_DEBUG(compressed_size);

  {
    std::ofstream out_binary_file("lz4.dat", std::ofstream::binary);
    out_binary_file.write(compressed, compressed_size);
  }

  std::ifstream inf_compressed_binary_file("lz4.dat", std::ifstream::binary);

  int compressed_input_size;
  {
    inf_compressed_binary_file.seekg(0, inf_compressed_binary_file.end);
    compressed_input_size = inf_compressed_binary_file.tellg();
    inf_compressed_binary_file.clear();
    inf_compressed_binary_file.seekg(0, std::ios::beg);
  }
  VAR_DEBUG(compressed_input_size);

  std::vector<char> compressed_input_vec(compressed_input_size);
  inf_compressed_binary_file.read(compressed_input_vec.data(),
                                  compressed_input_vec.size());
  inf_compressed_binary_file.close();

  // ---- ---- ---- ----
  // ---- ---- ---- ----
  // decompress
  // ---- ---- ---- ----
  // ---- ---- ---- ----
  // NOTE: condition: decompressed_size is known
  int decompressed_data_size = input_size;
  std::vector<char> decompressed_vec(decompressed_data_size);
  int decompressed_size =
      LZ4_decompress_safe(compressed_input_vec.data(), decompressed_vec.data(),
                          compressed_input_vec.size(), decompressed_vec.size());
  if (decompressed_size < 0) {
    // FYI: [return code of LZ4\_decompress\_safe doesn't understand · Issue \#421 · lz4/lz4]( https://github.com/lz4/lz4/issues/421 )
    // NOTE: this return value: The value is a hint at where the error probably happened. In this case, it seems to be close to the end of input.
    std::cerr << "lz4 decompress failed with some bad reason: status="
              << decompressed_size << std::endl;
    return 1;
  }
  // NOTE: shrink beacuse new size <= old size
  decompressed_vec.resize(decompressed_size);
  VAR_DEBUG(decompressed_size);

  {
    std::ofstream out_file("lz4.txt");
    out_file.write(decompressed_vec.data(), decompressed_vec.size());
  }
  return 0;
}
