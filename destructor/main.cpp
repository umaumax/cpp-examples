#include <iostream>
#include <memory>
#include <string>

class Small {
 public:
  explicit Small(std::string msg) : msg_(msg) {}
  ~Small() { std::cout << "Small destructor:" << msg_ << std::endl; }

 private:
  std::string msg_;
};

class Big {
 public:
  explicit Big(std::string msg) : msg_(msg) {
    small0_ = std::unique_ptr<Small>(new Small(msg_ + "-0"));
    small1_ = std::unique_ptr<Small>(new Small(msg_ + "-1"));
    small2_ = std::unique_ptr<Small>(new Small(msg_ + "-2"));
    small3_ = std::unique_ptr<Small>(new Small(msg_ + "-3"));
  }
  ~Big() { std::cout << "Big destructor" << msg_ << std::endl; }

 private:
  std::unique_ptr<Small> small1_;
  std::unique_ptr<Small> small2_;
  std::unique_ptr<Small> small3_;
  std::unique_ptr<Small> small0_;
  std::string msg_;
};

int main(int argc, char* argv[]) {
  Big big1("1");
  Big big2("2");
  return 0;
}
