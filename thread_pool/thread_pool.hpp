#ifndef THREAD_POOL_HPP_INCLUDED__
#define THREAD_POOL_HPP_INCLUDED__

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t);
  // NOTE: init function of thread pool
  ThreadPool(size_t, std::function<void(int)>);
  ~ThreadPool();

  template <class F, class... Args>
  auto Exec(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers_;
  // the task queue
  std::queue<std::function<void()>> tasks_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads) : ThreadPool(threads, nullptr) {}
// NOTE: init function of thread pool
inline ThreadPool::ThreadPool(size_t threads, std::function<void(int)> init_func) : stop_(false) {
  for (size_t i = 0; i < threads; ++i)
    workers_.emplace_back([this, init_func, i] {
      if (init_func) init_func(i);
      for (;;) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) break;
          task = std::move(tasks_.front());
          tasks_.pop();
        }
        task();
      }
    });
}
// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (auto &&worker : workers_) worker.join();
}

// add new work item to the pool
template <class F, class... Args>
auto ThreadPool::Exec(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task                    = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    // don't allow enqueueing after stopping the pool
    if (stop_) throw std::runtime_error("enqueue on stopped ThreadPool");
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}
#endif  // THREAD_POOL_HPP_INCLUDED__
