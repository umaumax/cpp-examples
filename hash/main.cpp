#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

TEST(hash, commutative_property_normal) {
  int n = 128;
  std::vector<int> vec(n);
  for (int i = 0; i < n; i++) {
    vec[i] = i;
  }

  std::map<std::size_t, int> hash_ret_map;

  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << vec[i] << ",";
    }
    std::size_t ret = std::hash<std::string>{}(ss.str());
    // std::cout << ss.str() << std::endl;
    hash_ret_map[ret]++;
  }

  {
    // NOTE: to confirm no 可換性（commutative property; 交換性質）
    std::rotate(vec.begin(), vec.begin() + 1, vec.end());
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << vec[i] << ",";
    }
    std::size_t ret = std::hash<std::string>{}(ss.str());
    // std::cout << ss.str() << std::endl;
    hash_ret_map[ret]++;
  }

  std::cerr << "hash : count" << std::endl;
  for (auto& v : hash_ret_map) {
    std::size_t hash_value = v.first;
    int count              = v.second;
    std::cerr << hash_value << " : " << count << std::endl;
    if (count > 2) {
      std::cerr << "[Failed] same hash value found!" << std::endl;
    }
    EXPECT_EQ(count, 1);
  }
}

TEST(hash, float_double_precision_5_normal) {
  int n = 128;
  std::vector<float> vec_a(n);
  std::vector<double> vec_b(n);
  for (int i = 0; i < n; i++) {
    vec_a[i] = std::sqrt(static_cast<float>(i));
    vec_b[i] = std::sqrt(static_cast<double>(i));
  }
  std::size_t ret_a;
  std::size_t ret_b;

  int precision = 5;
  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << std::setprecision(precision) << vec_a[i] << ",";
    }
    ret_a = std::hash<std::string>{}(ss.str());
  }
  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << std::setprecision(precision) << vec_b[i] << ",";
    }
    ret_b = std::hash<std::string>{}(ss.str());
  }

  EXPECT_EQ(ret_a, ret_b);
}

TEST(hash, float_double_precision_6_error) {
  int n = 128;
  std::vector<float> vec_a(n);
  std::vector<double> vec_b(n);
  for (int i = 0; i < n; i++) {
    vec_a[i] = std::sqrt(static_cast<float>(i));
    vec_b[i] = std::sqrt(static_cast<double>(i));
  }
  std::size_t ret_a;
  std::size_t ret_b;

  int precision = 6;
  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << std::setprecision(precision) << vec_a[i] << ",";
    }
    ret_a = std::hash<std::string>{}(ss.str());
  }
  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss << std::setprecision(precision) << vec_b[i] << ",";
    }
    ret_b = std::hash<std::string>{}(ss.str());
  }

  EXPECT_NE(ret_a, ret_b);
}

TEST(hash, binary_stream_normal) {
  int n = 128;
  std::vector<int> vec_a(n);
  std::vector<int> vec_b(n);
  for (int i = 0; i < n; i++) {
    vec_a[i] = i;
    vec_b[i] = i;
  }
  std::size_t ret_a;
  std::size_t ret_b;

  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss.write(reinterpret_cast<const char*>(&vec_a[i]), sizeof(int));
    }
    ret_a = std::hash<std::string>{}(ss.str());
  }
  {
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
      ss.write(reinterpret_cast<const char*>(&vec_b[i]), sizeof(int));
    }
    ret_b = std::hash<std::string>{}(ss.str());
  }

  EXPECT_EQ(ret_a, ret_b);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
