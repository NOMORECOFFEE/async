#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED 1

// boost headers
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/functional/generation/make_fused_procedure.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include <async/type_traits.hpp>

// declaration, forward
template<class T>
struct RemoveRef;

template<class SignT>
struct ParameterTypes;

template<class SeqT>
struct ArgumentsListTransform;

template<int id>
struct ArgumentsList;

template<int id>
struct AsyncState;

template<typename SignT, int id>
struct Arguments
{
    typedef typename AsyncParameterTypes<SignT>::Type Type;

    boost::optional<Type> value;
};

template<int i, class SignT>
Arguments<SignT, i> &getReference(Arguments<SignT, i> &theRef)
{
    return theRef;
}

template<int id, typename PtrT>
struct TaskWithContinuation
{
    typedef boost::fusion::fused_procedure<TaskWithContinuation> Type;

    explicit TaskWithContinuation(PtrT const &theState)
        : m_state( theState )
    {
    }

    template<typename SeqT>
    void operator()(SeqT theSeq) const
    {
        getReference<id>( m_state->m_arguments ).value = theSeq;

        int volatile *counter = &(m_state->m_counter);

        if (0 == __sync_fetch_and_sub(counter, 1))
        {
            invokeContinuation( m_state );
        }
    }
private:
    PtrT m_state;
};

template<int id, class PtrT>
typename TaskWithContinuation<id, PtrT>::Type inline
buildContinuationTask(PtrT const &theState)
{
    typedef typename TaskWithContinuation<id, PtrT>::Type Type;
    return static_cast<Type>(TaskWithContinuation<id, PtrT>( theState ));
}

template<class PtrT> inline
void invokeContinuation(PtrT theState)
{
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 5, "async/async.hpp"))
#include BOOST_PP_ITERATE()

#elif BOOST_PP_IS_ITERATING

#include <async/preprocessor/iterate_define.hpp>

template<>
struct ArgumentsList<ASYNC_PP_ITERATION>
{
    template<ASYNC_PP_typename_A>
    struct Type;
};

template<ASYNC_PP_typename_A>
struct ArgumentsList<ASYNC_PP_ITERATION>::Type
    : BOOST_PP_ENUM_PARAMS(ASYNC_PP_ITERATION, ArgumentsListTransform<void(ASYNC_PP_A)>::Type)
{
};

template<ASYNC_PP_typename_A>
struct ArgumentsListTransform<void(ASYNC_PP_A)>
{
    #define BOOST_PP_LOCAL_MACRO(n) \
        typedef Arguments<BOOST_PP_CAT(A, n), n> BOOST_PP_CAT(Type, n);

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()
};

template<>
struct AsyncState<ASYNC_PP_ITERATION>
{
    template<ASYNC_PP_typename_A, class ContinuationF>
    struct Type;
};

template<ASYNC_PP_typename_A, class ContinuationF>
struct AsyncState<ASYNC_PP_ITERATION>::Type
{
    mutable int m_count;
    ContinuationF m_callback;

    typename ArgumentsList<ASYNC_PP_ITERATION>::template Type<ASYNC_PP_A> m_arguments;

    explicit Type(ContinuationF theCallback)
        : m_count( ASYNC_PP_ITERATION )
        , m_callback( theCallback )
        , m_arguments()
    {
    }
};

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void callAsync(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void callAsync(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    asyncInvoke<ASYNC_PP_T>(boost::ref( theService ), ASYNC_PP_a, theService.wrap( onComplite ));
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    typedef typename AsyncState<ASYNC_PP_ITERATION>::template Type<ASYNC_PP_T, ContinuationF> Type;

    boost::shared_ptr<
        Type
    > state( boost::make_shared<Type>( onComplite ) );
    
    #define BOOST_PP_LOCAL_MACRO(n) \
        theService.post(boost::bind(BOOST_PP_CAT(a, n), buildContinuationTask<n>(state)));

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()
}

#endif // ASYNC_HPP_INCLUDED
