#include <iostream>
#include <memory>
#include <string>

#include "animal.hpp"
#include "cat.hpp"
#include "dog.hpp"

int main(int argc, char* argv[]) {
  AnimalFactory factory;
  factory.AddGenerator("dog", std::unique_ptr<Generator>(new DogGenerator));
  factory.AddGenerator("cat", std::unique_ptr<Generator>(new CatGenerator));
  if (argc <= 1) {
    std::cerr << argv[0] << " <dog | cat>" << std::endl;
    return -1;
  }
  std::string key(argv[1]);
  std::unique_ptr<Animal> animal = factory.Generate(key);
  if (animal == nullptr) {
    std::cerr << "[ERROR]: invalid key:" << key << std::endl;
    return -1;
  }
  animal->Say();
  return 0;
}
