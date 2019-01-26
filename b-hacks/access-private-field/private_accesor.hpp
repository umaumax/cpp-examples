#ifndef PRIVATE_ACCESOR_HPP_INCLUDED__
#define PRIVATE_ACCESOR_HPP_INCLUDED__

// [Calling private method in C\+\+ \- Stack Overflow]( https://stackoverflow.com/questions/6873138/calling-private-method-in-c )
template <typename TagType>
struct result {
  static TagType ptr;
};
// NOTE: create private pointer content variable
template <typename TagType>
TagType result<TagType>::ptr;

// NOTE: this assign pointer value to previous private pointer content variable
template <typename TagType, TagType p>
struct rob : result<TagType> {
  struct filler {
    // NOTE: constructor
    filler() { result<TagType>::ptr = p; }
  };
  static filler filler_obj;
};
// NOTE: create assigner to run constructor
template <typename TagType, TagType p>
typename rob<TagType, p>::filler rob<TagType, p>::filler_obj;

// NOTE: [Naive std::function implementation \| Shahar Mike's Web Spot]( https://shaharmike.com/cpp/naive-std-function/ )
// NOTE: split type to other types
template <typename, typename>
class class_type_helper;  // no definition
template <typename Class, typename ReturnValue, typename... Args>
class class_type_helper<Class, ReturnValue(Args...)> {
 public:
  using type        = ReturnValue (Class::*)(Args...);
  using return_type = ReturnValue;
  using arg_type    = void (*)(Args...);
};

// NOTE: create private method pointer content variable and decl function to access it
#define DECL_PRIVATE_METHOD_ACCESSOR(name, class_name, method_name, func_type)                                                                 \
  template class rob<class_type_helper<class_name, func_type>::type, &class_name::method_name>;                                                \
  template <class... Args>                                                                                                                     \
  class_type_helper<class_name, func_type>::return_type PRIVATE_METHOD_CALL_##name##_##method_name(class_name &x, Args &&... args) {           \
    return (class_type_helper<class_name, func_type>::return_type)((x).*result<class_type_helper<class_name, func_type>::type>::ptr)(args...); \
  }

// NOTE: create private member pointer content variable and decl function to access it
#define DECL_PRIVATE_MEMBER_ACCESSOR(name, class_name, member_name, member_type) \
  template class rob<member_type class_name::*, &class_name::member_name>;       \
  member_type &PRIVATE_MEMBER_ACCESS_##name##_##member_name(class_name &obj) { return obj.*result<member_type class_name::*>::ptr; }

#endif  // PRIVATE_ACCESOR_HPP_INCLUDED__
