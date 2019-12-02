#ifndef ANIMAL_HPP_INCLUDED__
#define ANIMAL_HPP_INCLUDED__

#include <algorithm>
#include <map>
#include <memory>

class Animal {
 public:
  virtual ~Animal() {}
  virtual void Say() = 0;
};

class Generator {
 public:
  virtual ~Generator() {}
  virtual std::unique_ptr<Animal> Generate() = 0;
};

class AnimalFactory {
 public:
  AnimalFactory() {}
  bool AddGenerator(const std::string& key,
                    std::unique_ptr<Generator> generator) {
    generator_map_.insert(std::make_pair(key, std::move(generator)));
    return true;
  }
  std::unique_ptr<Animal> Generate(const std::string& key) {
    auto&& it = generator_map_.find(key);
    if (it == generator_map_.end()) {
      return nullptr;
    }
    return generator_map_[key]->Generate();
  }

 protected:
  std::map<std::string, std::unique_ptr<Generator>> generator_map_;
};

#endif  // ANIMAL_HPP_INCLUDED__
