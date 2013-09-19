#include <async/async_fwd.hpp>

template<typename RandomAccessIteratorT>
void merge(
      RandomAccessIteratorT theBegin
    , RandomAccessIteratorT theMiddle
    , RandomAccessIteratorT theEnd
    , boost::function<void()> theCallback
{
    std::inplace_merge(theBegin, theMiddle, theEnd);
    theCallback();
}

template<typename RandomAccessIteratorT>
void sort(
      RandomAccessIteratorT theBegin
    , RandomAccessIteratorT theEnd
    , boost::function<void()> theCallback
{
    size_t size = theEnd - theBegin;
    
    if (size <= 4096) // block size 4k bytes
    {
        std::sort(theBegin, theEnd)
    }
    else
    {
        RandomAccessIteratorT middle = theBegin + size / 2;
        
        async<void(), void()>(
              boost::bind(&sort<RandomAccessIteratorT>, theBegin, middle, _1)
            , boost::bind(&sort<RandomAccessIteratorT>, middle, theEnd, _1)
            , boost::bind(&merge<RandomAccessIteratorT>, theBegin, theEnd, theCallback)
        );
    }
}
