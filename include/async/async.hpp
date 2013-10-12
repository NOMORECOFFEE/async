#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED 1

// boost headers
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/functional/generation/make_fused_procedure.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

// declaration, forward
template<class T>
struct RemoveRef;

template<typename SignT>
struct ParameterTypes;

template<typename SignT, int id>
struct ArgumentsList
{
    typedef typename ParameterTypes<SignT>::Type Type;

    boost::optional<Type> value;
};

template<int id, SignT>
ArgumentsList<SignT, id> &getReference(ArgumentsList<SignT, id> &theRef)
{
    return theRef;
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 10, "async.hpp"))
#include BOOST_PP_ITERATE()

#elif BOOST_PP_IS_ITERATING

#include <async/preprocessor/iterate_define.hpp>

template<ASYNC_PP_typename_T, ContinuationF>
struct AsyncState
{
    mutable int m_count;
    ContinuationF m_callback;
  
    ArgumentsList<ASYNC_PP_T> m_arguments;

    explicit AsyncState(ContinuationF theCallback)
        : m_count( ASYNC_PP_ITERATION )
        , m_callback( theCallback )
        , m_arguments()
    {
    }
}

#define ASYNC_PP_base_from_member_DEF(z, n, type) \
    ArgumentsList<BOOST_PP_CAT(type, n), n>

template<ASYNC_PP_typename_T>
struct ArgumentsList
    : BOOST_PP_REPEAT(ASYNC_PP_ITERATION, ASYNC_PP_base_from_member_DEF, T)
{
};

#undef ASYNC_PP_base_from_member_DEF

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
}

template<int id, class PtrT>
typename TaskWithContinuation<id, PtrT>::Type inline
buildContinuationTask(PtrT const &theState)
{
    return TaskWithContinuation<id, PtrT>( theState );
}

template<class AsyncState>
void invokeContinuation(boost::shared_ptr<AsyncState> &theState)
{
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    async_invoke(theService, ASYNC_PP_a, theService.wrap( onComplite ));   
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void async_invoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    typedef async_state<ASYNC_PP_A, ContinuationF>;
    
    boost::shared_ptr<
        async_state<ASYNC_PP_A, ContinuationF>
    > state = boost::make_shared( onComplite );
    
    async_invoke<ASYNC_PP_ITERATION>(
        theService, boost::fusion::make_vector(ASYNC_PP_a), state
    );

}

template<int i, typename SeqT, typename PtrT> inline
void async_invoke(boost::asio::io_service &theService, SeqT theSeq, PtrT theState)
{
    using boost::fusion::at;

    theService.post(
        boost::bind(at<i>(theSeq), buildContinuationTask<i>(theState)
    );

    async_invoke<i-1>(theService, t, theState);
}

#endif // ASYNC_HPP_INCLUDED
