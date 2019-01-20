#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <limits>

int main(int argc, char* argv[]) {
  // clang-format off
  class ResultChecker {
   public:
    ResultChecker(std::string dirname, std::string title) : dirname_(dirname), title_(title), filepath_(dirname + "/" + title + ".log"), check_flag_(false), th_(10e-8), ok_flag_(false) {
      const int dir_err = system((std::string("mkdir -p ") + dirname_).c_str());
      if (-1 == dir_err) {
        std::cerr << "Failed to mkdir: " << dirname_ << std::endl;
        return;
      }
      {
        std::ifstream ifs(filepath_);
        if (ifs.is_open()) {
          check_flag_ = true;
          fs_.open(filepath_);
        } else {
          fs_.open(filepath_, std::ios::in | std::ios::out | std::fstream::trunc);
        }
      }
      if (!fs_.is_open()) {
        std::cerr << "Failed to open: " << filepath_ << std::endl;
        return;
      }
      ok_flag_ = true;
      fs_.precision(std::numeric_limits<float>::digits10);
      fs_.precision(std::numeric_limits<double>::digits10);
    }
    bool Check(std::string key, int v) {
      if (!ok_flag_) return false;
      int array[1] = {v};
      return Check(key, array, 1, 1);
    }
    bool Check(std::string key, float v) {
      if (!ok_flag_) return false;
      float array[1] = {v};
      return Check(key, array, 1, 1);
    }
    bool Check(std::string key, double v) {
      if (!ok_flag_) return false;
      double array[1] = {v};
      return Check(key, array, 1, 1);
    }
    bool Check(std::string key, int* p, std::size_t size, std::size_t step = 1) {
      if (!ok_flag_) return false;
      std::replace(key.begin(), key.end(), ' ', '_');
      std::replace(key.begin(), key.end(), '\n', '_');
      bool result_flag = true;
      if (check_flag_) {
        std::string god_key;
        std::size_t god_size;
        std::size_t god_step;
        fs_ >> god_key;
        fs_ >> god_size;
        fs_ >> god_step;
        int god_value;
        std::string message = "";
        if (key != god_key) {
          message += [&]() {
            char buff[200];
            std::snprintf(buff, sizeof(buff), "[key != god_key: (%s != %s)]", key.c_str(), god_key.c_str());
            return std::string(buff);
          }();
        }
        if (size != god_size) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[size != god_size: (%zu != %zu)]", size, god_size);
            return std::string(buff);
          }();
        }
        if (step != god_step) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[step != god_step: (%zu != %zu)]", step, god_step);
            return std::string(buff);
          }();
        }
        if (message == "") {
          for (std::size_t i = 0; i < size; i += step) {
            fs_ >> god_value;
            auto& value = p[i];
            if (std::abs(god_value - value) > th_) {
              result_flag = false;
              message += [&]() {
                char buff[100];
                std::snprintf(buff, sizeof(buff), "[value != god_value: (%d != %d)]", value, god_value);
                return std::string(buff);
              }();
              break;
            }
          }
        }
        if (message != "") {
          result_flag = false;
        }
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: " << (result_flag ? "PASS" : "ERROR") << " " << message << std::endl;
      } else {
        fs_ << key << " ";
        fs_ << size << " ";
        fs_ << step << " ";
        for (std::size_t i = 0; i < size; i += step) {
          fs_ << p[i] << " ";
        }
        fs_ << "\n";
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: "
                  << "CREATE" << std::endl;
      }
      return result_flag;
    }
    bool Check(std::string key, float* p, std::size_t size, std::size_t step = 1) {
      if (!ok_flag_) return false;
      std::replace(key.begin(), key.end(), ' ', '_');
      std::replace(key.begin(), key.end(), '\n', '_');
      bool result_flag = true;
      if (check_flag_) {
        std::string god_key;
        std::size_t god_size;
        std::size_t god_step;
        fs_ >> god_key;
        fs_ >> god_size;
        fs_ >> god_step;
        float god_value;
        std::string message = "";
        if (key != god_key) {
          message += [&]() {
            char buff[200];
            std::snprintf(buff, sizeof(buff), "[key != god_key: (%s != %s)]", key.c_str(), god_key.c_str());
            return std::string(buff);
          }();
        }
        if (size != god_size) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[size != god_size: (%zu != %zu)]", size, god_size);
            return std::string(buff);
          }();
        }
        if (step != god_step) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[step != god_step: (%zu != %zu)]", step, god_step);
            return std::string(buff);
          }();
        }
        if (message == "") {
          for (std::size_t i = 0; i < size; i += step) {
            fs_ >> god_value;
            auto& value = p[i];
            if (std::abs(god_value - value) > th_) {
              result_flag = false;
              message += [&]() {
                char buff[100];
                std::snprintf(buff, sizeof(buff), "[value != god_value: (%f != %f)]", value, god_value);
                return std::string(buff);
              }();
              break;
            }
          }
        }
        if (message != "") {
          result_flag = false;
        }
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: " << (result_flag ? "PASS" : "ERROR") << " " << message << std::endl;
      } else {
        fs_ << key << " ";
        fs_ << size << " ";
        fs_ << step << " ";
        for (std::size_t i = 0; i < size; i += step) {
          fs_ << p[i] << " ";
        }
        fs_ << "\n";
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: "
                  << "CREATE" << std::endl;
      }
      return result_flag;
    }
    bool Check(std::string key, double* p, std::size_t size, std::size_t step = 1) {
      if (!ok_flag_) return false;
      std::replace(key.begin(), key.end(), ' ', '_');
      std::replace(key.begin(), key.end(), '\n', '_');
      bool result_flag = true;
      if (check_flag_) {
        std::string god_key;
        std::size_t god_size;
        std::size_t god_step;
        fs_ >> god_key;
        fs_ >> god_size;
        fs_ >> god_step;
        double god_value;
        std::string message = "";
        if (key != god_key) {
          message += [&]() {
            char buff[200];
            std::snprintf(buff, sizeof(buff), "[key != god_key: (%s != %s)]", key.c_str(), god_key.c_str());
            return std::string(buff);
          }();
        }
        if (size != god_size) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[size != god_size: (%zu != %zu)]", size, god_size);
            return std::string(buff);
          }();
        }
        if (step != god_step) {
          message += [&]() {
            char buff[100];
            std::snprintf(buff, sizeof(buff), "[step != god_step: (%zu != %zu)]", step, god_step);
            return std::string(buff);
          }();
        }
        if (message == "") {
          for (std::size_t i = 0; i < size; i += step) {
            fs_ >> god_value;
            auto& value = p[i];
            if (std::abs(god_value - value) > th_) {
              result_flag = false;
              message += [&]() {
                char buff[100];
                std::snprintf(buff, sizeof(buff), "[value != god_value: (%lf != %lf)]", value, god_value);
                return std::string(buff);
              }();
              break;
            }
          }
        }
        if (message != "") {
          result_flag = false;
        }
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: " << (result_flag ? "PASS" : "ERROR") << " " << message << std::endl;
      } else {
        fs_ << key << " ";
        fs_ << size << " ";
        fs_ << step << " ";
        for (std::size_t i = 0; i < size; i += step) {
          fs_ << p[i] << " ";
        }
        fs_ << "\n";
        std::cerr << "# RESULT_CHECK[" << title_ << "][" << key << "]: "
                  << "CREATE" << std::endl;
      }
      return result_flag;
    }
    bool Check(std::string key, std::vector<int> v, std::size_t step = 1) {
      if (!ok_flag_) return false;
      return Check(key, v.data(), v.size(), step);
    }
    bool Check(std::string key, std::vector<float> v, std::size_t step = 1) {
      if (!ok_flag_) return false;
      return Check(key, v.data(), v.size(), step);
    }
    bool Check(std::string key, std::vector<double> v, std::size_t step = 1) {
      if (!ok_flag_) return false;
      return Check(key, v.data(), v.size(), step);
    }

   private:
    std::string dirname_;
    std::string title_;
    std::string filepath_;
    bool check_flag_;
    std::fstream fs_;
    double th_;
    bool ok_flag_;
  };
  // clang-format on

  ResultChecker result_checker("result_dir/check_log", "main_func");
  result_checker.Check("one_float", 1.23f);
  float fs[2] = {1.1f, 1.2f};
  result_checker.Check("floats", fs, 2);
  const int n = 100;
  std::vector<float> vn(n);
  for (int i = 0; i < n; i++) {
    vn[i] = i;
  }
  const int step = 4;
  result_checker.Check("floats with step", vn, step);
  return 0;
}
