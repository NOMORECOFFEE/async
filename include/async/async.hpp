#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED 1

// boost headers
#include <boost/asio/io_service.hpp>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/functional/generation/make_fused_procedure.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/iteration/self.hpp>

// std headers
#include <atomic>
#include <tuple>
#include <utility>
#include <memory>

#include <async/type_traits.hpp>

// declaration, forward
template<typename SignT>
struct Arguments;

template<typename ... SignT>
using ArgumentsList = std::tuple<Arguments<SignT> ...>;

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

template<typename SignT>
struct Arguments
{
    typedef typename AsyncParameterTypes<SignT>::type type;

    boost::optional<type> value;
};

template<int id, typename PtrT>
struct TaskWithContinuation
{
    typedef void result_type;

    explicit TaskWithContinuation(PtrT theState) noexcept
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
        std::get<id>( m_state->m_arguments ).value = std::forward<SeqT>( theSeq );

        auto &counter = m_state->m_counter;

        if (0 == --counter)
        {
            invokeContinuation(m_state);
        }
    }

    PtrT m_state;
};

template<int id, class PtrT> inline
auto buildContinuationTask(PtrT &&theState)
{
    return TaskWithContinuation<id, typename std::decay<PtrT>::type>( std::forward<PtrT>(theState) );
}

template<class PtrT> inline
void invokeContinuation(PtrT &&theState)
{
    invokeContinuation(theState->m_callback, theState->m_arguments);
}

template<typename ... SignT, typename ContinuationF> inline
void invokeContinuation(ContinuationF &&theCallback, ArgumentsList<SignT ...> theArguments)
{
    invokeContinuation(std::forward<ContinuationF>(theCallback), std::move(theArguments), std::index_sequence_for<SignT ...>());
}

template<typename ContinuationF, typename ArgsT, size_t ... I> inline
void invokeContinuation(ContinuationF &&theCallback, ArgsT &&theArguments, std::index_sequence<I...>)
{
     boost::fusion::make_fused_procedure( std::forward<ContinuationF>(theCallback) )(
         std::tuple_cat( std::get<I>(std::forward<ArgsT>(theArguments)).value.get() ... )
    );
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 5, "async/async.hpp"))
#include BOOST_PP_ITERATE()

#elif BOOST_PP_IS_SELFISH

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF &&onComplete);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF> inline
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF &&onComplete);

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void async(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF &&onComplete)
{
    asyncInvoke<ASYNC_PP_T>(boost::ref( theService ), ASYNC_PP_a, std::forward<ContinuationF>( onComplete ));
}

template<ASYNC_PP_typename_T, ASYNC_PP_typename_A, typename ContinuationF>
void asyncInvoke(boost::asio::io_service &theService, ASYNC_PP_A_a, ContinuationF &&onComplete)
{
    using type = typename AsyncState<ASYNC_PP_T>::template Type<ContinuationF>;

    auto state( std::make_shared<type>( std::forward<ContinuationF>( onComplete ) ) );

    #define BOOST_PP_LOCAL_MACRO(n)          \
        theService.post(                     \
            [task = std::move(BOOST_PP_CAT(a, n)), args = buildContinuationTask<n>(state)](){ task(args); } \
        );

    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(ASYNC_PP_ITERATION))

    #include BOOST_PP_LOCAL_ITERATE()
}

#elif BOOST_PP_IS_ITERATING

#include <async/preprocessor/iterate_define.hpp>
#define BOOST_PP_INDIRECT_SELF "async/async.hpp"
#include BOOST_PP_INCLUDE_SELF()

#endif // ASYNC_HPP_INCLUDED
