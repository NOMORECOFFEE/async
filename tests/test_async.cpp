// boost headers
#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>

#include <async/async.hpp>

using namespace boost;
using namespace boost::asio;

BOOST_AUTO_TEST_SUITE(async_tests_suite)

void onComplite0()
{
}

void onComplite1(int theValue, int theValueIn)
{
    BOOST_REQUIRE_EQUAL(theValue, theValueIn);
}

void simpleFunc0(function<void()> theFunc)
{
    theFunc();
}

void SimpleFunc1(int theValue, function<void(int)> theFunc)
{
    theFunc( theValue );
}

BOOST_AUTO_TEST_CASE(null_arguments_call)
{
    io_service service;

    async<void()>(boost::ref( service ), bind(simpleFunc0, _1), onComplite0);

    service.run();
}

BOOST_AUTO_TEST_CASE(single_arguments_call)
{
    io_service service;

    async<void(int)>(boost::ref( service ), bind(SimpleFunc1, 10, _1), bind(onComplite1, 10, _1));

    service.run();
}

BOOST_AUTO_TEST_SUITE_END()
