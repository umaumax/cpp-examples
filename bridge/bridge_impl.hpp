#pragma once

#include <iostream>
#include <memory>

class IntReader : public Reader {
 public:
  IntReader() {}
  ~IntReader() {}
  std::shared_ptr<void> Read(std::shared_ptr<void> input_ptr) {
    return input_ptr;
  }
};

class IntToFloatConverter : public Converter {
 public:
  IntToFloatConverter() {}
  ~IntToFloatConverter() {}
  std::shared_ptr<void> Convert(std::shared_ptr<void> input_ptr) {
    auto& input = static_cast<int&>(*reinterpret_cast<int*>(input_ptr.get()));
    auto output_ptr = std::make_shared<float>(0);
    auto& output =
        static_cast<float&>(*reinterpret_cast<float*>(output_ptr.get()));
    output = static_cast<float>(input);
    return output_ptr;
  }
};

class InPlaceIntToFloatConverter : public Converter {
 public:
  InPlaceIntToFloatConverter() : output_ptr_(std::make_shared<float>(0)) {}
  ~InPlaceIntToFloatConverter() {}
  std::shared_ptr<void> Convert(std::shared_ptr<void> input_ptr) {
    auto& input = static_cast<int&>(*reinterpret_cast<int*>(input_ptr.get()));
    auto& output =
        static_cast<float&>(*reinterpret_cast<float*>(output_ptr_.get()));
    output = static_cast<float>(input);
    return output_ptr_;
  }
  std::shared_ptr<float> output_ptr_;
};

class FloatWriter : public Writer {
 public:
  FloatWriter() {}
  ~FloatWriter() {}
  void Write(std::shared_ptr<void> input_ptr) {
    auto& input =
        static_cast<float&>(*reinterpret_cast<float*>(input_ptr.get()));
    std::cout << input << std::endl;
  }
};

template <class T>
class PrimitiveTypeCallback : public CallbackBase {
 public:
  PrimitiveTypeCallback(Reader& reader, Converter& converter, Writer& writer)
      : reader_(reader), converter_(converter), writer_(writer) {
    callback_ = [&](const T& input) {
      std::shared_ptr<void> input_ptr = std::make_shared<T>(input);
      writer_.Write(converter_.Convert(reader_.Read(input_ptr)));
    };
  }
  ~PrimitiveTypeCallback() {}
  void* GetCallback() { return reinterpret_cast<void*>(&callback_); }

 private:
  Reader& reader_;
  Converter& converter_;
  Writer& writer_;

  std::function<void(const T& input)> callback_;
};

template <class T>
class VectorProceduralListener : public Listener {
 public:
  VectorProceduralListener(std::vector<T>& vec, CallbackBase& callback_base)
      : vec_(vec), callback_base_(callback_base) {}
  ~VectorProceduralListener() {}

  void Listen() {
    auto& callback =
        (*(std::function<void(const int&)>*)callback_base_.GetCallback());
    for (auto&& e : vec_) {
      callback(e);
    }
  }

 private:
  std::vector<T>& vec_;
  CallbackBase& callback_base_;
};
