#include <iostream>

#include <limits.h>
#include <chrono>
#include <cstdio>
#include <map>
#include <string>

void f(int n) {
  /*
  #include <limits.h>
  #include <chrono>
  #include <cstdio>
  #include <map>
  #include <string>
  */
  // clang-format off
  class MonoStopWatch { using time_type = std::chrono::system_clock::time_point; public: MonoStopWatch() { Clear(); } void Start() { start_time_ = now(); } void Stop() { end_time_ = now(); update(ElapsedTime()); } double ElapsedTime() { return std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_).count() / 1000.0; } void Print(const std::string& message) { std::fprintf(stderr, "%48s (ms) [%10llu] sum:%11.4lf, ave:%11.4lf, (min,max)=(%11.4lf,%11.4lf), now:%11.4lf\n", message.c_str(), callee_cnt_, sum_elapsed_time_, ave_elapsed_time_, min_elapsed_time_, max_elapsed_time_, pre_elapsed_time_); }; void Clear() { callee_cnt_       = 0; /* NOTE: REQUIRED: #include <limits.h> */ min_elapsed_time_ = std::numeric_limits<double>::max(); max_elapsed_time_ = std::numeric_limits<double>::min(); sum_elapsed_time_ = 0.0; ave_elapsed_time_ = 0.0; pre_elapsed_time_ = 0.0; }; private: time_type now() { return std::chrono::system_clock::now(); } void update(double elapsed_time) { callee_cnt_++; min_elapsed_time_ = elapsed_time < min_elapsed_time_ ? elapsed_time : min_elapsed_time_; max_elapsed_time_ = max_elapsed_time_ < elapsed_time ? elapsed_time : max_elapsed_time_; sum_elapsed_time_ += elapsed_time; ave_elapsed_time_ = sum_elapsed_time_ / static_cast<double>(callee_cnt_); pre_elapsed_time_ = elapsed_time; } long long unsigned int callee_cnt_; double min_elapsed_time_; double max_elapsed_time_; double sum_elapsed_time_; double ave_elapsed_time_; double pre_elapsed_time_; time_type start_time_; time_type end_time_; };
  class StopWatch { public: StopWatch() {} void SetPrefixMessage(std::string message) { message_ = message; } void Clear(std::string key) { getMonoStopWatch(key).Clear(); }; void Start(std::string key) { getMonoStopWatch(key).Start(); }; void Stop(std::string key) { getMonoStopWatch(key).Stop(); }; void Print(std::string key) { getMonoStopWatch(key).Print(message_ + ":" + key); }; void PrintAll() { for (auto& x : mono_stop_watch_map_) { Print(x.first); } } private: MonoStopWatch& getMonoStopWatch(std::string key) { return mono_stop_watch_map_[key]; } std::string message_ = "#"; std::map<std::string, MonoStopWatch> mono_stop_watch_map_; };
  // clang-format on

  static StopWatch stopwatch;

  stopwatch.Clear("for-loop");
  stopwatch.Start("total");
  int sum = 0;
  for (int i = 0; i < n; i++) {
    stopwatch.Start("for-loop");
    sum += i;
    stopwatch.Stop("for-loop");
  }
  stopwatch.Stop("total");

  // NOTE: you can print each key or all keys
  //   stopwatch.Print("total");
  //   stopwatch.Print("for-loop");
  stopwatch.PrintAll();
}

int main(int argc, char* argv[]) {
  f(1000000);
  f(1000000);
  f(1000000);
  f(1000000);
  return 0;
}
