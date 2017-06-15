// boost headers
#include <boost/test/unit_test.hpp>

#include <async/async.hpp>
#include <boost/optional.hpp>

#include <functional>

using namespace boost;
using namespace boost::asio;

using namespace std;
using namespace std::placeholders;

BOOST_AUTO_TEST_SUITE(async_tests_suite)

void onComplite0()
{
}

void onComplite2(int theValue, int theValueIn)
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

    async<void()>(std::ref( service ), bind(simpleFunc0, _1), onComplite0);

    service.run();
}

BOOST_AUTO_TEST_CASE(single_arguments_call)
{
    io_service service;

    async<void(int)>(std::ref( service ), bind(SimpleFunc1, 10, _1), bind(onComplite2, 10, _1));

    service.run();
}

BOOST_AUTO_TEST_CASE(double_arguments_call)
{
    io_service service;

    async<void(int), void(int)>(
        std::ref( service ),
        bind(SimpleFunc1, 10, _1),
        bind(SimpleFunc1, 10, _1),
        onComplite2
    );

    service.run();
}

BOOST_AUTO_TEST_SUITE_END()
