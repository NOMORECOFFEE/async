#include <boost/test/unit_test.hpp>

#include <boost/fusion/functional/adapter/unfused.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/function.hpp>

using namespace boost;
using namespace boost::fusion;

struct UnfusedCall
{
    typedef void result_type;

    template<class SeqT>
    void operator()(SeqT theSeq) const
    {
    }
};


BOOST_AUTO_TEST_CASE(TestUnfusedCall)
{
    function<void (int, int, int)> c = unfused<UnfusedCall>();

    c(1, 2, 3);
}
