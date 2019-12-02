#ifndef DOG_HPP_INCLUDED__
#define DOG_HPP_INCLUDED__

#include <iostream>

#include "animal.hpp"

class Dog : public Animal {
 public:
  Dog() {}
  ~Dog() {}
  void Say() override;
};

class DogGenerator : public Generator {
 public:
  DogGenerator() {}
  ~DogGenerator() {}
  std::unique_ptr<Animal> Generate() override {
    return std::unique_ptr<Animal>(new Dog());
  };
};

#endif  // DOG_HPP_INCLUDED__
