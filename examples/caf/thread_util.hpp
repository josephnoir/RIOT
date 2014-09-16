
#ifndef CAF_THREAD_UTILS_HPP
#define CAF_THREAD_UTILS_HPP

#include <utility>

// decay copy (from type_traits)

template <class T>
inline typename std::decay<T>::type decay_copy(T&& t) {
    return std::forward<T>(t);
}

// make indices (from clang __tuple)

template <size_t...>
struct indices { };

template <size_t S,class IntTuple, size_t E>
struct make_indices_impl;

template <size_t S, size_t... Indices, size_t E> 
struct make_indices_impl<S, indices<Indices...>, E> {
  using type = typename make_indices_impl<S+1,indices<Indices...,S>, E>::type;
};

template <size_t E, size_t... Indices>
struct make_indices_impl<E, indices<Indices...>, E> {
 using type = indices<Indices...>;
};

template <size_t E, size_t S = 0>
struct make_indices {
  using type = typename make_indices_impl<S,indices<>,E>::type;
};

#endif // CAF_THREAD_UTILS_HPP
