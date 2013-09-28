#include <boost/test/unit_test.hpp>

#include <boost/fusion/functional/adapter/unfused.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/functional/lightweight_forward_adapter.hpp>

using namespace boost;
using namespace boost::fusion;

struct UnfusedCall
{
    typedef void result_type;

    template<class SeqT>
    void operator()(SeqT const &theSeq) const
    {
    }
};


BOOST_AUTO_TEST_CASE(TestUnfusedCall)
{
    lightweight_forward_adapter< unfused<UnfusedCall> > c;

    c(1, 2, 3);
}
