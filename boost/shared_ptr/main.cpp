#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <memory>

class Test {
 public:
  Test() { std::cout << "  hello" << std::endl; }
  ~Test() { std::cout << "  bye" << std::endl; }
};

// FYI: [c\+\+ \- Conversion from boost::shared\_ptr to std::shared\_ptr? \- Stack Overflow]( https://stackoverflow.com/questions/6326757/conversion-from-boostshared-ptr-to-stdshared-ptr )
template <class T>
void do_release(class boost::shared_ptr<T> const&, T*) {}
template <class T>
class std::shared_ptr<T> to_std_shared_ptr(
    class boost::shared_ptr<T> const& p) {
  return std::shared_ptr<T>(p.get(), boost::bind(&do_release<T>, p, _1));
}

// NOTE: remvoe const to assign std::shared_ptr<void>
template <class T>
class std::shared_ptr<T> to_std_shared_ptr(
    class boost::shared_ptr<const T> const& p) {
  return std::shared_ptr<T>(const_cast<T*>(p.get()),
                            boost::bind(&do_release<const T>, p, _1));
}

int main() {
  std::cout << "# main start" << std::endl;

  std::cout << std::endl;

  std::cout << "# 1 start" << std::endl;
  {
    //
    boost::shared_ptr<Test> boost_shared_ptr{new Test()};
  }
  std::cout << "# 1 end" << std::endl;

  std::cout << std::endl;

  // NOTE: boost::shared_ptr to std::shared_ptr example
  {
    std::cout << "# 2 start" << std::endl;
    std::shared_ptr<Test> std_shared_ptr;
    {
      boost::shared_ptr<Test> boost_shared_ptr{new Test()};
      std_shared_ptr = to_std_shared_ptr(boost_shared_ptr);
    }
    std::cout << "# 2 end" << std::endl;
  }

  std::cout << std::endl;

  // NOTE: multi boost::shared_ptr to std::shared_ptr example
  {
    std::cout << "# 2-2 start" << std::endl;
    std::shared_ptr<Test> std_shared_ptr;
    {
      boost::shared_ptr<Test> boost_shared_ptr{new Test()};
      boost::shared_ptr<Test> boost_shared_ptr2 = boost_shared_ptr;
      std_shared_ptr = to_std_shared_ptr(boost_shared_ptr);
    }
    std::cout << "# 2-2 end" << std::endl;
  }

  std::cout << std::endl;

  // NOTE: boost::shared_ptr to std::shared_ptr<void> example
  {
    std::cout << "# 3 start" << std::endl;
    std::shared_ptr<void> std_shared_ptr;
    {
      boost::shared_ptr<Test> boost_shared_ptr{new Test()};
      std_shared_ptr = to_std_shared_ptr(boost_shared_ptr);
    }
    std::cout << "# 3 end" << std::endl;
  }

  std::cout << std::endl;

  // NOTE: boost::shared_ptr<const xxx> to std::shared_ptr<void> example
  {
    std::cout << "# 3-2 start" << std::endl;
    std::shared_ptr<void> std_shared_ptr;
    {
      const Test* test = new Test();
      boost::shared_ptr<const Test> boost_shared_ptr{test};
      std_shared_ptr = to_std_shared_ptr(boost_shared_ptr);
    }
    std::cout << "# 3-2 end" << std::endl;
  }

  std::cout << std::endl;

  // NOTE: std::shared_ptr<boost::shared_ptr<xxx>> to std::shared_ptr<void> example
  {
    std::cout << "# 3-3 start" << std::endl;
    std::shared_ptr<void> std_shared_ptr;
    {
      boost::shared_ptr<Test> boost_shared_ptr{new Test()};
      std_shared_ptr =
          std::make_shared<decltype(boost_shared_ptr)>(boost_shared_ptr);
    }
    std::cout << "# 3-3 end" << std::endl;
  }

  std::cout << std::endl;

  std::cout << "# main end" << std::endl;

  // NOTE: standard example
  //   boost::shared_ptr<int> p1{new int{1}};
  //   std::cout << *p1 << '\n';
  //   boost::shared_ptr<int> p2{p1};
  //   p1.reset(new int{2});
  //   std::cout << *p1.get() << '\n';
  //   p1.reset();
  //   std::cout << std::boolalpha << static_cast<bool>(p2) << '\n';
}
