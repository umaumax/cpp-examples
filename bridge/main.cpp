#include <iostream>
#include <string>
#include <vector>

#include "bridge.hpp"

int main(int argc, char* argv[]) {
  IntReader int_reader;
  IntToFloatConverter int_to_float_converter;
  InPlaceIntToFloatConverter in_place_int_to_float_converter;
  FloatWriter float_writer;

  Reader& reader = int_reader;
  // NOTE: choose in-place converter or not-in-place converter
  //   Converter& converter = int_to_float_converter;
  Converter& converter = in_place_int_to_float_converter;
  Writer& writer       = float_writer;

  // NOTE: pattern 1
  //   auto input_ptr = std::make_shared<int>(123);
  //   writer.Write(converter.Convert(reader.Read(input_ptr)));

  PrimitiveTypeCallback<int> primitive_type_callback(reader, converter, writer);
  CallbackBase& callback_base = primitive_type_callback;
  // NOTE: pattern 2
  //   auto& callback             = (*(std::function<void(const int&)>*)callbackBase.GetCallback());
  //   callback(123);

  // NOTE: pattern 3
  std::vector<int> vec{1, 2, 3};
  VectorProceduralListener<int> vector_procedural_listener(vec, callback_base);

  Listener& listener = vector_procedural_listener;
  listener.Listen();
  return 0;
}
