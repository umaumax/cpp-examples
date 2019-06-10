#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

int add(int a, int c) { return a + c; }
int hello(std::string name) {
  std::cout << "Hello " << name << std::endl;
  return 0;
}
void str_upper(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  std::cout << "str_upper called: result=" << str << std::endl;
}

int main(int argc, char* argv[]) {
  boost::asio::io_service io_service;
  boost::asio::io_service::work work(io_service);
  std::vector<std::thread> thread_pool;

  int thread_pool_n = 3;
  for (size_t i = 0; i < thread_pool_n; i++) {
    thread_pool.emplace_back([&io_service] { io_service.run(); });
  }

  // NOTE: one task example
  {
    // NOTE: with return value
    boost::packaged_task<int> pt(boost::bind(add, 1, 2));
    // NOTE: io_service post requirements
    // The handler to be called. The io_service will make a copy of the handler object as required.
    // The function signature of the handler must be: void handler();
    io_service.post(std::bind([&] { pt(); }));
    boost::unique_future<int> uf = pt.get_future();
    std::cout << "add(1,2)=" << uf.get() << std::endl;
  }

  // NOTE: one task example with copying ref args
  {
    std::string msg = "nanoha";
    boost::packaged_task<void> pt(boost::bind(str_upper, msg));
    io_service.post(std::bind([&] { pt(); }));
    boost::unique_future<void> uf = pt.get_future();
    uf.get();
    std::cout << "msg:" << msg << std::endl;
  }

  // NOTE: multi tasks example
  int task_n = 5;
  std::vector<boost::packaged_task<int>> pts(task_n);
  for (size_t i = 0; i < task_n; i++) {
    // NOTE: without return value
    io_service.post(std::bind(hello, "boost"));
    // NOTE: with return value
    pts[i] = boost::packaged_task<int>(boost::bind(add, i, i));
    io_service.post(std::bind([&, i] { pts[i](); }));
  }

  std::cout << "This is Main Thread!" << std::endl;

  for (size_t i = 0; i < task_n; i++) {
    std::cout << "add() result = " << pts[i].get_future().get() << std::endl;
  }

  io_service.stop();
  for (std::thread& t : thread_pool) {
    t.join();
  }
}
