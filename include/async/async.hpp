#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED 1

// boost headers
#include <boost/optional/optional_fwd.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

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

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 10, "async.hpp"))
#include BOOST_PP_ITERATE()

#elif BOOST_PP_IS_ITERATING

#include <async/preprocessor/iterate_define.hpp>

template<ASYNC_PP_typename_T, ContinuationF>
struct async_state
{
    mutable int m_count;
    ContinuationF m_callback;
  
    explicit async_state(ContinuationF theCallback)
        : m_count( ASYNC_PP_ITERATION )
        , m_callback( theCallback )
    {
    }
}

template<int i, typename AsyncStateT>
struct task_with_continuation
{
    explicit task_with_continuation(boost::shared_ptr<AsyncStateT> theState)
        : m_state( theState )
    {
    }
    
    template<typename SeqT>
    void operator()(SeqT theSeq) const
    {
        int volatile *counter = &(m_state->m_counter);
        
        if (0 == __sync_fetch_and_sub(counter, 1))
        {
            invoke_continuation( m_state );
        }
    }
private:
    boost::shared_ptr<AsyncStateT> m_state;
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    async_invoke(theService, ASYNC_PP_a, theService.wrap( onComplite ));   
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void async_invoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF onComplite)
{
    typedef async_state<ASYNC_PP_A, ContinuationF>;
    
    boost::shared_ptr<
        async_state<ASYNC_PP_A, ContinuationF>
    > state = boost::make_shared( onComplite );
    
    ASYNC_PP_POST_TASKS
}

#endif // ASYNC_HPP_INCLUDED
