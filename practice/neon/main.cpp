#include <arm_neon.h>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

// [NEONコプロセッサ｜Cortex\-A編]( https://www.aps-web.jp/academy/ca/14/ )

float calc(float* __restrict fdata1, float* __restrict fdata2, std::size_t size) {
  float res = 0.0;
  unsigned long i;

  for (i = 0; i < size; i++) {
    res += fdata1[i] + fdata2[i];
  }
  return res;
}

float neon_calc(float* __restrict fdata1, float* __restrict fdata2, std::size_t size) {
  float res = 0.0;
  unsigned long i;

  float32x4_t f_data1, f_data2;
  float32x4_t f_res;

  for (i = 0; i < size / 4; i++) {
    f_data1 = vld1q_f32(fdata1);
    f_data2 = vld1q_f32(fdata2);
    f_res   = vaddq_f32(f_data1, f_data2);
    res += vgetq_lane_f32(f_res, 0) + vgetq_lane_f32(f_res, 1) + vgetq_lane_f32(f_res, 2) + vgetq_lane_f32(f_res, 3);
    fdata1 += 4;
    fdata2 += 4;
  }
  return res;
}

int main(int argc, char* argv[]) {
  // clang-format off
  class MonoStopWatch { using time_type = std::chrono::system_clock::time_point; public: MonoStopWatch() { Clear(); } void Start() { start_time_ = now(); } void Stop() { end_time_ = now(); update(ElapsedTime()); } double ElapsedTime() { return std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_).count() / 1000.0; } void Print(const std::string& message) { std::fprintf(stderr, "%48s (ms) [%10llu] sum:%11.4lf, ave:%11.4lf, (min,max)=(%11.4lf,%11.4lf), now:%11.4lf\n", message.c_str(), callee_cnt_, sum_elapsed_time_, ave_elapsed_time_, min_elapsed_time_, max_elapsed_time_, pre_elapsed_time_); }; void Clear() { callee_cnt_       = 0; /* NOTE: REQUIRED: #include <limits.h> */ min_elapsed_time_ = std::numeric_limits<double>::max(); max_elapsed_time_ = std::numeric_limits<double>::min(); sum_elapsed_time_ = 0.0; ave_elapsed_time_ = 0.0; pre_elapsed_time_ = 0.0; }; private: time_type now() { return std::chrono::system_clock::now(); } void update(double elapsed_time) { callee_cnt_++; min_elapsed_time_ = elapsed_time < min_elapsed_time_ ? elapsed_time : min_elapsed_time_; max_elapsed_time_ = max_elapsed_time_ < elapsed_time ? elapsed_time : max_elapsed_time_; sum_elapsed_time_ += elapsed_time; ave_elapsed_time_ = sum_elapsed_time_ / static_cast<double>(callee_cnt_); pre_elapsed_time_ = elapsed_time; } long long unsigned int callee_cnt_; double min_elapsed_time_; double max_elapsed_time_; double sum_elapsed_time_; double ave_elapsed_time_; double pre_elapsed_time_; time_type start_time_; time_type end_time_; };
  class StopWatch { public: StopWatch() {} void SetPrefixMessage(std::string message) { message_ = message; } void Clear(std::string key) { getMonoStopWatch(key, false).Clear(); }; void Start(std::string key) { getMonoStopWatch(key, false).Start(); }; void Stop(std::string key) { getMonoStopWatch(key).Stop(); }; void Print(std::string key) { getMonoStopWatch(key).Print(message_ + ":" + key); }; void PrintAll() { for (auto& x : mono_stop_watch_map_) { Print(x.first); } } private: MonoStopWatch& getMonoStopWatch(std::string key, bool assert_flag=true) { if(assert_flag) { auto it=mono_stop_watch_map_.find(key); assert((it!=mono_stop_watch_map_.end()) && "no exist key found"); } return mono_stop_watch_map_[key]; } std::string message_ = "#"; std::map<std::string, MonoStopWatch> mono_stop_watch_map_; };
  // clang-format on
  static StopWatch stopwatch;

  int n = 1000000 * 4;
  int m = 10;
  float ret;

  ret = 0.0;
  for (int i = 0; i < m; i++) {
    stopwatch.Start("normal");
    std::vector<float> v1(n);
    std::vector<float> v2(n);
    for (int i = 0; i < n; i++) {
      v1[i] = i;
      v2[i] = i;
    }
    ret += calc(v1.data(), v2.data(), n);
    stopwatch.Stop("normal");
  }
  std::printf("calc=%f\n", ret);

  ret = 0.0;
  for (int i = 0; i < m; i++) {
    stopwatch.Start("neon");
    std::vector<float> v1(n);
    std::vector<float> v2(n);
    for (int i = 0; i < n; i++) {
      v1[i] = i;
      v2[i] = i;
    }
    ret += neon_calc(v1.data(), v2.data(), n);
    stopwatch.Stop("neon");
  }
  std::printf("calc=%f\n", ret);

  stopwatch.PrintAll();
  return 0;
}
