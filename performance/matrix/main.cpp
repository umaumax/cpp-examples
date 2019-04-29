#include <algorithm>
#include <iostream>
#include <string>

#include "func_io.hpp"

// NOTE: for mkdir
#include <sys/stat.h>

void matrix_mul(const int* a, const int* b, int* c, const int aw, const int ah,
                const int bw) {
  const char* matrix_mul_target_env_char = std::getenv("MATRIX_MUL_TARGET");
  std::string matrix_mul_target_env      = std::string(
      matrix_mul_target_env_char != nullptr ? matrix_mul_target_env_char : "");

  // NOTE: 本来の関数の処理
  const int bh = aw;
  const int cw = bw;
  const int ch = ah;
  if (matrix_mul_target_env == "") {
    for (int i = 0; i < ch; i++) {
      for (int j = 0; j < cw; j++) {
        c[i * cw + j] = 0;
        for (int k = 0; k < aw; k++) {
          c[i * cw + j] += a[i * aw + k] * b[k * bw + j];
        }
      }
    }
  } else {
    // NOTE: blockingを利用したバージョン
    const int ii_step = 2;
    const int jj_step = 2;
    const int kk_step = 2;
    for (int ii = 0; ii < ch; ii += ii_step) {
      const int ii_end = std::min(ii + ii_step, ch);
      for (int jj = 0; jj < cw; jj += jj_step) {
        const int jj_end = std::min(jj + jj_step, cw);
        for (int kk = 0; kk < aw; kk += kk_step) {
          const int kk_end = std::min(kk + kk_step, aw);
          for (int i = ii; i < ii_end; ++i) {
            for (int j = jj; j < jj_end; ++j) {
              if (kk == 0) {
                c[i * cw + j] = 0;
              }
              for (int k = kk; k < kk_end; ++k) {
                c[i * cw + j] += a[i * aw + k] * b[k * bw + j];
              }
            }
          }
        }
      }
    }
  }

  static std::size_t call_cnt = 0;
  call_cnt++;
  const char* func_test_output_env_char = std::getenv("FUNC_TEST_OUTPUT");
  std::string func_test_output_env      = std::string(
      func_test_output_env_char != nullptr ? func_test_output_env_char : "");
  if (func_test_output_env != "") {
    func_test_output_env += "/";
    int status = mkdir(func_test_output_env.c_str(), 0777);
  }
  std::string filename =
      func_test_output_env + ".tmp_out." + std::to_string(call_cnt);
  const char* func_test_env_char = std::getenv("FUNC_TEST");
  std::string func_test_env =
      std::string(func_test_env_char != nullptr ? func_test_env_char : "");
  const char* func_test_save_env_char = std::getenv("FUNC_TEST_SAVE");
  std::string func_test_save_env      = std::string(
      func_test_save_env_char != nullptr ? func_test_save_env_char : "");
  const char* func_test_gzip_env_char = std::getenv("FUNC_TEST_GZIP");
  std::string func_test_gzip_env      = std::string(
      func_test_gzip_env_char != nullptr ? func_test_gzip_env_char : "1");
  if (func_test_env != "") {
    std::ifstream ifs(filename);
    if (ifs) {
      // NOTE: decompress
      std::stringstream decompressed;
      if (func_test_gzip_env != "") {
        boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
        out.push(boost::iostreams::gzip_decompressor());
        out.push(ifs);
        boost::iostreams::copy(out, decompressed);
      } else {
        decompressed << ifs.rdbuf();
      }

      // boost::archive::text_iarchive tia(decompressed);
      boost::archive::binary_iarchive tia(decompressed);
      // NOTE: 計算結果の比較用にstd::vectorを利用
      std::size_t c_size;
      tia >> c_size;
      // bia >> c_size;
      std::vector<int> c_ans_vec(c_size);
      tia >> boost::serialization::make_array(c_ans_vec.data(), c_size);
      for (int i = 0; i < c_size; i++) {
        if (c_ans_vec[i] != c[i]) {
          std::cout << "[" << i << "](got,ans)=(" << c[i] << "," << c_ans_vec[i]
                    << ")" << std::endl;
        }
      }
      return;
    }
  }

  // NOTE: save output
  if (func_test_save_env != "") {
    std::stringstream ss;
    {
      // boost::archive::text_oarchive toa(ss);
      boost::archive::binary_oarchive toa(ss);
      std::size_t c_size = cw * ch;
      toa << c_size;
      toa << boost::serialization::make_array(c, c_size);
    }

    // NOTE: compress
    std::stringstream compressed;
    if (func_test_gzip_env != "") {
      boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
      out.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(
          boost::iostreams::gzip::best_compression)));
      out.push(ss);
      boost::iostreams::copy(out, compressed);
    } else {
      compressed << ss.str();
    }

    std::fstream ofs(filename, std::fstream::out | std::ios::binary);
    // std::fstream::out | std::ios::binary | std::fstream::app);
    ofs << compressed.str();
  }
}

int main(int argc, char* argv[]) {
  // NOTE: Min example
  {
    const int aw  = 4;
    const int ah  = 3;
    const int bw  = 3;
    const int bh  = aw;
    const int cw  = bw;
    const int ch  = ah;
    int a[ah][aw] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    int b[bh][bw] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
    int c[ah][bw];
    int ans[ah][bw] = {{70, 80, 90}, {158, 184, 210}, {246, 288, 330}};
    matrix_mul((int*)a, (int*)b, (int*)c, aw, ah, bw);
  }
  {
    const int aw = 400;
    const int ah = 300;
    const int bw = 300;
    const int bh = aw;
    const int cw = bw;
    const int ch = ah;
    int a[ah][aw];
    int b[bh][bw];
    int c[ah][bw];
    for (int i = 0; i < ah * aw; i++) {
      ((int*)a)[i] = i;
    }
    for (int i = 0; i < bh * bw; i++) {
      ((int*)b)[i] = i;
    }
    matrix_mul((int*)a, (int*)b, (int*)c, aw, ah, bw);
  }

  // NOTE: ret
  // std::cout << "ret:" << std::endl;
  // for (int j = 0; j < ch; j++) {
  // for (int i = 0; i < cw; i++) {
  // std::cout << c[j][i] << ", ";
  // }
  // std::cout << std::endl;
  // }

  // NOTE: ans
  // std::cout << "ans:" << std::endl;
  // std::cout << "ret(ans)" << std::endl;
  // for (int j = 0; j < ch; j++) {
  // for (int i = 0; i < cw; i++) {
  // std::cout << c[j][i];
  // if (ans[j][i] != c[j][i]) {
  // std::cout << "(" << ans[j][i] << ")";
  // }
  // std::cout << ", ";
  // }
  // std::cout << std::endl;
  // }
  return 0;
}
