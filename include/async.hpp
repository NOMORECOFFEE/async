#ifndef ASYNC_INCLUDED_HPP
#define ASYNC_INCLUDED_HPP 1

// boost headers
#include <boost/optional/optional_fwd.hpp>

// declaration, forward
template<class T>
struct RemoveRef;

template<typename SignT>
struct ParameterTypes;

struct RemoveRef
{
    template<class SingT> struct result;
    
    template<class T>
    struct result<void(T)>
    {
        typedef typename RemoveRef<T>::Type type;    
    };
    
    template<class T>
    typename RemoveRef<T>::type operator()(T theValue) const
    {
        return theValue
    }
    
};

struct SetPack
{
    template<class SignT>
    struct result
    {
        typedef typename ParameterTypes<SignT>::Type type;
    };
}



#endif // ASYNC_INCLUDED_HPP
