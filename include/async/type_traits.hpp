#ifndef TYPE_TRAITS_HPP_INCLUDED
#define TYPE_TRAITS_HPP_INCLUDED 1

// boost headers
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/container/vector/convert.hpp>

namespace extension
{
    template<class SignT, template <class T> class TransformationT>
    struct AsyncParameterTypes
    {
        typedef typename boost::mpl::transform<
            typename boost::function_types::parameter_types<SignT>::type,
            TransformationT<boost::mpl::placeholders::_>
        >::type Type;
    };
} // namespace extension

template<class SignT>
struct AsyncParameterTypes
{
    typedef typename boost::fusion::result_of::as_vector<
        typename extension::AsyncParameterTypes<SignT, boost::remove_reference>::Type
    >::type Type;
};

#endif // TYPE_TRAITS_HPP_INCLUDED
