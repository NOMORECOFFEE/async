#ifndef TYPE_TRAITS_HPP_INCLUDED
#define TYPE_TRAITS_HPP_INCLUDED 1

// boost headers
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/container/vector/convert.hpp>

template<class SignT>
struct AsyncParameterTypes
{
    typedef typename boost::fusion::result_of::as_vector<typename boost::mpl::transform<
        typename boost::function_types::parameter_types<SignT>::type,
        boost::remove_reference<boost::mpl::placeholders::_>
    >::type>::type Type;
};

#endif // TYPE_TRAITS_HPP_INCLUDED
