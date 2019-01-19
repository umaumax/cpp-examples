#ifndef PRIVATE_ACCESOR_HPP_INCLUDED__
#define PRIVATE_ACCESOR_HPP_INCLUDED__

// [Calling private method in C\+\+ \- Stack Overflow]( https://stackoverflow.com/questions/6873138/calling-private-method-in-c )
template <typename TagType>
struct result {
  /* export it ... */
  static TagType ptr;
};

template <typename TagType>
TagType result<TagType>::ptr;

template <typename TagType, TagType p>
struct rob : result<TagType> {
  /* fill it ... */
  struct filler {
    filler() { result<TagType>::ptr = p; }
  };
  static filler filler_obj;
};

template <typename TagType, TagType p>
typename rob<TagType, p>::filler rob<TagType, p>::filler_obj;

template <typename, typename>
class class_type_helper;  // no definition
template <typename Class, typename ReturnValue, typename... Args>
class class_type_helper<Class, ReturnValue(Args...)> {
 public:
  using type        = ReturnValue (Class::*)(Args...);
  using return_type = ReturnValue;
  using arg_type    = void (*)(Args...);
};

#define DECL_PRIVATE_METHOD_ACCESSOR(class_name, method_name, func_type)                                                                       \
  template class rob<class_type_helper<class_name, func_type>::type, &class_name::method_name>;                                                \
  template <class... Args>                                                                                                                     \
  class_type_helper<class_name, func_type>::return_type PRIVATE_METHOD_CALL_##class_name##_##method_name(class_name &x, Args &&... args) {     \
    return (class_type_helper<class_name, func_type>::return_type)((x).*result<class_type_helper<class_name, func_type>::type>::ptr)(args...); \
  }

#define DECL_PRIVATE_MEMBER_ACCESSOR(class_name, member_name, member_type) \
  template class rob<member_type class_name::*, &class_name::member_name>; \
  member_type &PRIVATE_MEMBER_ACCESS_##class_name##_##member_name(class_name &obj) { return obj.*result<member_type class_name::*>::ptr; }

#endif  // PRIVATE_ACCESOR_HPP_INCLUDED__
