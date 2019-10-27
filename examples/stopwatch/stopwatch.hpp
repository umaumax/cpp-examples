#ifndef STOPWATCH_HPP_INCLUDED__
#define STOPWATCH_HPP_INCLUDED__

#include <cassert>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace internal {
template <class T>
class Singleton {
 public:
  static inline T& GetInstance() {
    static T instance;
    return instance;
  }

 private:
  Singleton()  = default;
  ~Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&)                 = delete;
  Singleton& operator=(Singleton&&) = delete;
};

class IndentManager {
 public:
  IndentManager() : level_(0) {}

  void Push() { level_++; }
  // NOTE: level_ must be >= 0
  void Pop() { level_--; }
  std::string String() {
    std::stringstream ss;
    const std::string head = "|";
    ss << std::setfill(' ') << std::setw(level_ + head.size()) << head;
    return ss.str();
  }
  int GetLevel() { return level_; }

 private:
  int level_;
};
}  // namespace internal

class MonoStopWatch {
  using time_type = std::chrono::system_clock::time_point;

 public:
  MonoStopWatch() : prefix_("") { Clear(); }

  void Start() { start_time_ = now(); }
  bool Toggle() {
    if (start_time_.time_since_epoch().count() == 0) {
      Start();
      return true;
    }
    Stop();
    return false;
  }
  void Stop() {
    end_time_ = now();
    update(ElapsedTime());
    start_time_ = {};
  }
  double ElapsedTime() {
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time_ -
                                                                 start_time_)
               .count() /
           1000.0;
  }
  void Print(const std::string& message) {
    std::stringstream ss;
    ss << std::left << std::setfill('-') << std::setw(8) << prefix_
       << std::right << std::setfill('-') << std::setw(44) << message;
    std::string prefix = ss.str();

    std::fprintf(stderr,
                 "%s (ms) [%10llu] sum:%11.3lf, ave:%11.3lf, "
                 "(min,max)=(%11.3lf,%11.3lf), now:%11.3lf\n",
                 prefix.c_str(), callee_cnt_, sum_elapsed_time_,
                 ave_elapsed_time_, min_elapsed_time_, max_elapsed_time_,
                 pre_elapsed_time_);
  };
  void Clear() {
    callee_cnt_       = 0; /* NOTE: REQUIRED: #include <limits.h> */
    min_elapsed_time_ = std::numeric_limits<double>::max();
    max_elapsed_time_ = std::numeric_limits<double>::min();
    sum_elapsed_time_ = 0.0;
    ave_elapsed_time_ = 0.0;
    pre_elapsed_time_ = 0.0;
  };
  void SetPrefix(const std::string& prefix) { prefix_ = prefix; }

 private:
  time_type now() { return std::chrono::system_clock::now(); }
  void update(double elapsed_time) {
    callee_cnt_++;
    min_elapsed_time_ =
        elapsed_time < min_elapsed_time_ ? elapsed_time : min_elapsed_time_;
    max_elapsed_time_ =
        max_elapsed_time_ < elapsed_time ? elapsed_time : max_elapsed_time_;
    sum_elapsed_time_ += elapsed_time;
    ave_elapsed_time_ = sum_elapsed_time_ / static_cast<double>(callee_cnt_);
    pre_elapsed_time_ = elapsed_time;
  }
  long long unsigned int callee_cnt_;
  double min_elapsed_time_;
  double max_elapsed_time_;
  double sum_elapsed_time_;
  double ave_elapsed_time_;
  double pre_elapsed_time_;
  time_type start_time_;
  time_type end_time_;
  std::string prefix_;
};
class StopWatch {
 public:
  StopWatch() {}
  ~StopWatch() {
    if (delay_print_flag_ || !print_flag_) {
      PrintAll();
    }
  }

  void SetPrefixMessage(const std::string& message) { message_ = message; }
  // NOTE: for control print key order
  void Register(const std::string& key) { GetMonoStopWatch(key, false); };
  void Clear(const std::string& key) { GetMonoStopWatch(key, false).Clear(); };
  void Start(const std::string& key) {
    auto&& mono_stop_watch = GetMonoStopWatch(key, false);
    mono_stop_watch.Start();
    mono_stop_watch.SetPrefix(indent_manager_.String());

    indent_manager_.Push();
  };
  void Stop(const std::string& key) {
    auto&& mono_stop_watch = GetMonoStopWatch(key);
    mono_stop_watch.Stop();

    indent_manager_.Pop();
  };
  void Toggle(const std::string& key) {
    auto&& mono_stop_watch = GetMonoStopWatch(key, false);
    bool started           = mono_stop_watch.Toggle();
    if (started) {
      mono_stop_watch.SetPrefix(indent_manager_.String());
      indent_manager_.Push();
    } else {
      indent_manager_.Pop();
    }
  };
  void Print(const std::string& key) {
    print_flag_ = true;
    GetMonoStopWatch(key).Print(message_ + ":" + key);
  };
  void PrintAll() {
    print_flag_ = true;
    for (auto&& key : keys_) {
      Print(key);
    }
  }
  void SetDelayPrintFlag() { delay_print_flag_ = true; }

 private:
  MonoStopWatch& GetMonoStopWatch(const std::string& key,
                                  bool assert_flag = true) {
    auto&& it = mono_stop_watch_map_.find(key);
    if (it == mono_stop_watch_map_.end()) {
      keys_.emplace_back(key);
      assert(!assert_flag && "no exist key found");
    }
    return mono_stop_watch_map_[key];
  }
  std::string message_ = "#";
  std::map<std::string, MonoStopWatch> mono_stop_watch_map_;
  std::vector<std::string> keys_;
  bool print_flag_       = false;
  bool delay_print_flag_ = false;

  internal::IndentManager& indent_manager_ =
      internal::Singleton<internal::IndentManager>::GetInstance();
};

#endif  // STOPWATCH_HPP_INCLUDED__
