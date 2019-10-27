// clang-format off
class LocalHookCallback { public: LocalHookCallback() {} ~LocalHookCallback() { if (callback_) callback_(); } void SetCallback(std::function<void(void)> callback) { callback_ = callback; } private: std::function<void(void)> callback_; };
// clang-format on
// clang-format off
class MonoStopWatch { using time_type = std::chrono::system_clock::time_point; public: MonoStopWatch() { Clear(); } void Start() { start_time_ = now(); } void Stop() { end_time_ = now(); update(ElapsedTime()); } double ElapsedTime() { return std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_).count() / 1000.0; } void Print(const std::string& message) { std::fprintf(stderr, "%48s (ms) [%10llu] sum:%11.4lf, ave:%11.4lf, (min,max)=(%11.4lf,%11.4lf), now:%11.4lf\n", message.c_str(), callee_cnt_, sum_elapsed_time_, ave_elapsed_time_, min_elapsed_time_, max_elapsed_time_, pre_elapsed_time_); }; void Clear() { callee_cnt_       = 0; /* NOTE: REQUIRED: #include <limits.h> */ min_elapsed_time_ = std::numeric_limits<double>::max(); max_elapsed_time_ = std::numeric_limits<double>::min(); sum_elapsed_time_ = 0.0; ave_elapsed_time_ = 0.0; pre_elapsed_time_ = 0.0; }; private: time_type now() { return std::chrono::system_clock::now(); } void update(double elapsed_time) { callee_cnt_++; min_elapsed_time_ = elapsed_time < min_elapsed_time_ ? elapsed_time : min_elapsed_time_; max_elapsed_time_ = max_elapsed_time_ < elapsed_time ? elapsed_time : max_elapsed_time_; sum_elapsed_time_ += elapsed_time; ave_elapsed_time_ = sum_elapsed_time_ / static_cast<double>(callee_cnt_); pre_elapsed_time_ = elapsed_time; } long long unsigned int callee_cnt_; double min_elapsed_time_; double max_elapsed_time_; double sum_elapsed_time_; double ave_elapsed_time_; double pre_elapsed_time_; time_type start_time_; time_type end_time_; };
class StopWatch { public: StopWatch() {} void SetPrefixMessage(std::string message) { message_ = message; } void Clear(std::string key) { getMonoStopWatch(key, false).Clear(); }; void Start(std::string key) { getMonoStopWatch(key, false).Start(); }; void Stop(std::string key) { getMonoStopWatch(key).Stop(); }; void Print(std::string key) { getMonoStopWatch(key).Print(message_ + ":" + key); }; void PrintAll() { for (auto& x : mono_stop_watch_map_) { Print(x.first); } } private: MonoStopWatch& getMonoStopWatch(std::string key, bool assert_flag=true) { if(assert_flag) { auto it=mono_stop_watch_map_.find(key); assert((it!=mono_stop_watch_map_.end()) && "no exist key found"); } return mono_stop_watch_map_[key]; } std::string message_ = "#"; std::map<std::string, MonoStopWatch> mono_stop_watch_map_; };
// clang-format on
static StopWatch stopwatch;
const std::string stopwatch_total_key =
    std::string("[total]:") + std::string(__func__);
LocalHookCallback local_hook_callback;
local_hook_callback.SetCallback([&]() {
  std::printf("ret=%d\n", ret);
  stopwatch.Stop(stopwatch_total_key);
  stopwatch.PrintAll();
});

stopwatch.Start(stopwatch_total_key);
// NOTE: this block is for preventing name confliction
{
  ret = 123;
  return ret;
}
