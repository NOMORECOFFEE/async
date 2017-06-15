#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define ASYNC_PP_ITERATION                                                 \
    BOOST_PP_ITERATION()                                                   \
/**/

#define ASYNC_PP_typename_A                                                \
    BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, typename A)                   \
/**/

#define ASYNC_PP_typename_T                                                \
    BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, typename T)                   \
/**/

#define ASYNC_PP_typename_A_void                                           \
    BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(ASYNC_PP_ITERATION, typename A, void)
/**/

#define ASYNC_PP_A                                                          \
    BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, A)                             \
/**/

#define ASYNC_PP_T                                                          \
    BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, T)                             \
/**/

#define ASYNC_PP_a                                                          \
    BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, a)                             \
/**/


#define ASYNC_PP_A_ref                                                      \
    BOOST_PP_ENUM_BINARY_PARAMS(ASYNC_PP_ITERATION, A, & BOOST_PP_INTERCEPT)\
/**/

#define ASYNC_PP_A_const_ref                                                \
    BOOST_PP_ENUM_BINARY_PARAMS(ASYNC_PP_ITERATION, A, const& BOOST_PP_INTERCEPT)\
/**/

#define ASYNC_PP_A_a                                                        \
    BOOST_PP_ENUM_BINARY_PARAMS(ASYNC_PP_ITERATION, A, a)                   \
/**/

#define ASYNC_PP_A_ref_a                                                    \
    BOOST_PP_ENUM_BINARY_PARAMS(ASYNC_PP_ITERATION, A, & a)                 \
/**/

#define ASYNC_PP_A_const_ref_a                                              \
    BOOST_PP_ENUM_BINARY_PARAMS(ASYNC_PP_ITERATION, A, const& a)            \
/**/
