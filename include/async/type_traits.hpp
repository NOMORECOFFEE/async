#ifndef TYPE_TRAITS_HPP_INCLUDED
#define TYPE_TRAITS_HPP_INCLUDED 1

// boost headers
#include <boost/type_traits/remove_reference.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>

template<class T>
struct RemoveRef
{
    typedef typename boost::remove_reference<T>::type Type;
};

template<typename SignT>
struct ParameterTypes
{
    typedef typename boost::parameter_types<SignT>::type Type;
};

template<typename SignT, class TransformationT>
struct ParameterTypes
{
    typedef typename boost::fusion::transform<
        typename ParameterTypes<SignT>::Type, TransformationT
    >::type Type;
};

#endif // TYPE_TRAITS_HPP_INCLUDED
