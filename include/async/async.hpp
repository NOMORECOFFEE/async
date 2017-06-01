#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED 1

// boost headers
#include <boost/asio/io_service.hpp>
#include <boost/fusion/algorithm/iteration/accumulate.hpp>
#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/functional/generation/make_fused_procedure.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/iteration/self.hpp>
#include <boost/type_traits/remove_reference.hpp>

// std headers
#include <atomic>

#include <async/type_traits.hpp>

// declaration, forward
template<class SeqT>
struct ArgumentsListTransform;

template<typename ... SignT>
struct ArgumentsList;

template<typename ... SignT>
struct AsyncState
{
    template<typename ContinuationF>
    struct Type
    {
        std::atomic<int> m_counter;
        ContinuationF m_callback;

        ArgumentsList<SignT ...> m_arguments;

        explicit Type(ContinuationF theCallback)
            : m_counter( sizeof ... (SignT) )
            , m_callback( std::move(theCallback) )
            , m_arguments()
        {
        }
    };
};

template<typename SignT, int id>
struct Arguments
{
    typedef typename AsyncParameterTypes<SignT>::type type;

    boost::optional<type> value;
};

template<int id, class SignT> constexpr
Arguments<SignT, id> &getReference(Arguments<SignT, id> &theRef)
{
    return theRef;
}

template<int id, class SignT> constexpr
Arguments<SignT, id> const &getReference(Arguments<SignT, id> const &theRef)
{
    return theRef;
}

template<int id, typename PtrT>
struct TaskWithContinuation
{
    typedef void result_type;

    explicit TaskWithContinuation(PtrT theState)
        : m_state( std::move( theState ) )
    {
    }

    template<typename ...Args>
    void operator()(Args && ... theArgs) const {
        unfused(boost::in_place(std::forward<Args>(theArgs) ...));
    }

private:
    template<typename SeqT> inline
    void unfused(SeqT && theSeq) const {
        getReference<id>( m_state->m_arguments ).value = std::move( theSeq );

        auto &counter = m_state->m_counter;

        if (0 == --counter)
        {
            invokeContinuation( m_state );
        }
    }

    PtrT m_state;
};

template<int id, class PtrT>
TaskWithContinuation<id, PtrT> inline
buildContinuationTask(PtrT &&theState)
{
    return TaskWithContinuation<id, PtrT>( std::forward<PtrT>(theState) );
}


struct JoinArguments
{
    template<class SingT>
    struct result;

    template<class R, class T0, class T1>
    struct result<R(T0, T1)>
    {
        using type = decltype(boost::fusion::join(std::declval<T0>(), std::declval<T1>()));
    };

    template<class T0, class T1>
    typename result<void(T0 const&, T1 const&)>::type  operator()(T0 const &theAcc, T1 const &theValue) const
    {
        return boost::fusion::join(theAcc, theValue);
    }
};

template<class PtrT> inline
void invokeContinuation(PtrT theState)
{
    invokeContinuation(theState->m_callback, theState->m_arguments);
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 5, "async/async.hpp"))
#include BOOST_PP_ITERATE()

#elif BOOST_PP_IS_SELFISH

template<ASYNC_PP_typename_A>
struct ArgumentsList<ASYNC_PP_A>
    : BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, ArgumentsListTransform<void(ASYNC_PP_A)>::Type)
{
};

template<ASYNC_PP_typename_A>
struct ArgumentsListTransform<void(ASYNC_PP_A)>
{
    #define BOOST_PP_LOCAL_MACRO(n) \
        using BOOST_PP_CAT(Type, n) = Arguments<BOOST_PP_CAT(A, n), n>;

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()
};

template<ASYNC_PP_typename_T, typename ContinuationF> inline
void invokeContinuation(
    ContinuationF theCallback,
    ArgumentsList<ASYNC_PP_T> theArguments
)
{
    #define BOOST_PP_LOCAL_MACRO(n)                   \
       auto const &BOOST_PP_CAT(a, n) = (             \
            getReference<n>(theArguments).value.get() \
        );

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()

    boost::fusion::fused_procedure<ContinuationF> callback( theCallback );

    callback(
        boost::fusion::accumulate(
            boost::fusion::make_vector(ASYNC_PP_a),
            boost::fusion::make_vector(),
            JoinArguments()
        )
    );
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplete);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplete);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplete)
{
    asyncInvoke<ASYNC_PP_T>(boost::ref( theService ), ASYNC_PP_a, theService.wrap( onComplete ));
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplete)
{
    using Type = typename AsyncState<ASYNC_PP_T>::template Type<ContinuationF>;

    auto state( std::make_shared<Type>( onComplete ) );

    #define BOOST_PP_LOCAL_MACRO(n)          \
        theService.post(                     \
            [=](){ BOOST_PP_CAT(a, n)(buildContinuationTask<n>(std::move(state))); } \
        );

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()
}

#elif BOOST_PP_IS_ITERATING

#include <async/preprocessor/iterate_define.hpp>
#define BOOST_PP_INDIRECT_SELF "async/async.hpp"
#include BOOST_PP_INCLUDE_SELF()

#endif // ASYNC_HPP_INCLUDED
