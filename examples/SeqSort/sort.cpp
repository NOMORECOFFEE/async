#include "sort.hpp"
#include <boost/optional.hpp>

using namespace boost;
using namespace boost::asio;

void onComplite()
{
}

void SimpleFunc(function<void()> theFunc)
{
    theFunc();
}

void run()
{
    std::vector<unsigned char> a;

    io_service service;

    callAsync<void()>(boost::ref( service ), bind(SimpleFunc, _1), onComplite);
    //sort(boost::ref( service ), a.data(), a.data(), bind(onComplite));
}




