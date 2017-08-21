#ifndef TYPE_TRAITS_HPP_INCLUDED
#define TYPE_TRAITS_HPP_INCLUDED 1

// std headers
#include <type_traits>
#include <tuple>

template<class SignT>
struct AsyncParameterTypes;

template<typename R, typename ... Args>
struct AsyncParameterTypes<R(Args ...)>
{
    using type = std::tuple<typename std::decay<Args>::type ...>;
};

#endif // TYPE_TRAITS_HPP_INCLUDED
