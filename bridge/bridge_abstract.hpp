#pragma once

#include <memory>

class Reader {
 public:
  Reader() {}
  virtual ~Reader() {}
  virtual std::shared_ptr<void> Read(std::shared_ptr<void> input_ptr) = 0;
};

class Converter {
 public:
  Converter() {}
  virtual ~Converter() {}
  virtual std::shared_ptr<void> Convert(std::shared_ptr<void> input_ptr) = 0;
};

class Writer {
 public:
  Writer() {}
  virtual ~Writer() {}
  virtual void Write(std::shared_ptr<void> input_ptr) = 0;
};

class CallbackBase {
 public:
  CallbackBase() {}
  virtual ~CallbackBase() {}
  virtual void* GetCallback() = 0;
};

class Listener {
 public:
  Listener() {}
  virtual ~Listener() {}
  virtual void Listen() = 0;
};
