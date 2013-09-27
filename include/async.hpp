#ifndef ASYNC_INCLUDED_HPP
#define ASYNC_INCLUDED_HPP 1

// boost headers
#include <boost/function_types/parameter_types.hpp>
#include <boost/type_traits/remove_reference.hpp>

struct RemoveRef
{
    template<class SingT> struct result;
    
    template<class T>
    struct result<void(T)>
    {
        typedef typename boost::remove_reference<T>::type type;    
    };
    
    template<class T>
    typename result<void(T)>::type operator()(T theValue) const
    {
        return theValue
    }
    
};

struct SetPack
{
    template<class SignT>
    struct result
    {
        typedef typename boost::parameter_types<SignT>::type ParameterTypes;
        
        typedef boost::optional<
            typename boost::fusion::transform<ParameterTypes, RemoveRef>::type
        > type
    };
}



#endif // ASYNC_INCLUDED_HPP
