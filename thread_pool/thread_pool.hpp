#ifndef THREAD_POOL_HPP_INCLUDED__
#define THREAD_POOL_HPP_INCLUDED__

#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(int);
  // NOTE: init function of thread pool
  ThreadPool(int, std::function<void(int)>);
  ~ThreadPool();

  // NOTE: use std::invoke_result not std::result_of(C++17~)
  template <class F, class... Args>
  auto PushTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
  void Join();

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_flag_;
};

// NOTE: constructor just starts workers and wait tasks
inline ThreadPool::ThreadPool(int threads) : ThreadPool(threads, nullptr) {}
// NOTE: init function of thread pool
inline ThreadPool::ThreadPool(int threads, std::function<void(int)> init_func) : stop_flag_(false) {
  for (int i = 0; i < threads; ++i)
    workers_.emplace_back([this, init_func, i] {
      if (init_func) init_func(i);
      while (true) {
        std::function<void()> task;
        // NOTE: wait until this thread gets task
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_.wait(lock, [this] { return stop_flag_ || !tasks_.empty(); });
          if (stop_flag_ && tasks_.empty()) break;
          task = std::move(tasks_.front());
          tasks_.pop();
        }
        task();
      }
    });
}
// NOTE: destructor joins all threads if all tasks are completed
inline ThreadPool::~ThreadPool() { Join(); }
inline void ThreadPool::Join() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_flag_ = true;
  }
  condition_.notify_all();
  for (auto &&worker : workers_) worker.join();
}

// add new work item to the pool
template <class F, class... Args>
auto ThreadPool::PushTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task                    = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    assert(!stop_flag_ || "[assert] PushTask() called on stopped ThreadPool");
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}
#endif  // THREAD_POOL_HPP_INCLUDED__
