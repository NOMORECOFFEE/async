// boost headers
#include <boost/test/unit_test.hpp>

#include <async/async.hpp>
#include <boost/optional.hpp>

using namespace boost;
using namespace boost::asio;

using namespace std;

BOOST_AUTO_TEST_SUITE(async_tests_suite)

BOOST_AUTO_TEST_CASE(null_arguments_call)
{
    io_service service;

    async<void()>(std::ref( service ), [](auto theProcedure) { theProcedure(); }, []{});

    boost::system::error_code error;
    service.run(std::ref(error));
    BOOST_REQUIRE(!error);
}

BOOST_AUTO_TEST_CASE(single_arguments_call)
{
    io_service service;

    constexpr auto expect = 10;

    async<void(int)>(
        std::ref( service ),
        [=](auto theProcedure){
            theProcedure(expect);
        },
        [=](auto theValue){
            BOOST_REQUIRE_EQUAL(expect, theValue);
        }
    );

    boost::system::error_code error;
    service.run(std::ref(error));
    BOOST_REQUIRE(!error);
}

BOOST_AUTO_TEST_CASE(double_arguments_call)
{
    io_service service;

    constexpr auto expect = 10;

    async<void(int), void(int)>(
        std::ref( service ),
        [=](auto theProcedure){
            theProcedure(expect);
        },
        [=](auto theProcedure){
            theProcedure(expect);
        },
        [=](auto lhs, auto rhs){
            BOOST_REQUIRE_EQUAL(expect, lhs);
            BOOST_REQUIRE_EQUAL(expect, rhs);
        }
    );

    boost::system::error_code error;
    service.run(std::ref(error));
    BOOST_REQUIRE(!error);
}

BOOST_AUTO_TEST_SUITE_END()
