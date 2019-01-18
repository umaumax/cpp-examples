#ifndef PRIVATE_ACCESOR_HPP_INCLUDED__
#define PRIVATE_ACCESOR_HPP_INCLUDED__

// [Calling private method in C\+\+ \- Stack Overflow]( https://stackoverflow.com/questions/6873138/calling-private-method-in-c )
template <typename Tag>
struct result {
  /* export it ... */
  typedef typename Tag::type type;
  static type ptr;
};

template <typename Tag>
typename result<Tag>::type result<Tag>::ptr;

template <typename Tag, typename Tag::type p>
struct rob : result<Tag> {
  /* fill it ... */
  struct filler {
    filler() { result<Tag>::ptr = p; }
  };
  static filler filler_obj;
};

template <typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

//

#include <functional>
#define DECL_PRIVATE_METHOD_ACCESSOR(class_name, method_name, return_type, args_type)    \
  struct class_name##_##method_name {                                                    \
    using type = return_type(class_name::*) args_type;                                   \
  };                                                                                     \
  template class rob<class_name##_##method_name, &class_name::method_name>;              \
  template <class F, class... Args>                                                      \
  return_type PRIVATE_METHOD_CALL_##class_name##_##method_name(F &&x, Args &&... args) { \
    return (return_type)((x).*result<class_name##_##method_name>::ptr)(args...);         \
  }

#define DECL_PRIVATE_MEMBER_ACCESSOR(class_name, member_name, member_type)  \
  struct class_name##_##member_name {                                       \
    using type = member_type class_name::*;                                 \
  };                                                                        \
  template class rob<class_name##_##member_name, &class_name::member_name>; \
  member_type &PRIVATE_ACCESS_##class_name##_##member_name(class_name &obj) { return obj.*result<class_name##_##member_name>::ptr; }

#endif  // PRIVATE_ACCESOR_HPP_INCLUDED__
