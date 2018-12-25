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
#define DECL_PRIVATE_METHOD_ACCESSOR(class_name, target_name, target_type1, target_type2) \
  struct class_name##_##target_name {                                                     \
    using type = target_type1(class_name::*) target_type2;                                \
  };                                                                                      \
  template class rob<class_name##_##target_name, &class_name::target_name>;

#define DECL_PRIVATE_MEMBER_ACCESSOR(class_name, target_name, target_type)  \
  struct class_name##_##target_name {                                       \
    using type = target_type class_name::*;                                 \
  };                                                                        \
  template class rob<class_name##_##target_name, &class_name::target_name>; \
  target_type& PRIVATE_ACCESS_##class_name##_##target_name(class_name& obj) { return obj.*result<class_name##_##target_name>::ptr; }

#endif  // PRIVATE_ACCESOR_HPP_INCLUDED__
