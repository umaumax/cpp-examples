#ifndef CAT_HPP_INCLUDED__
#define CAT_HPP_INCLUDED__

#include <iostream>

#include "animal.hpp"

class Cat : public Animal {
 public:
  Cat() {}
  ~Cat() {}
  void Say() override;
};

class CatGenerator : public Generator {
 public:
  CatGenerator() {}
  ~CatGenerator() {}
  std::unique_ptr<Animal> Generate() override {
    return std::unique_ptr<Animal>(new Cat());
  };
};

#endif  // CAT_HPP_INCLUDED__
