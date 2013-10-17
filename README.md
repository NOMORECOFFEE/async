async
=====

examples:
template<typename RandomAccessIteratorT> inline
void merge(
      RandomAccessIteratorT theBegin
    , RandomAccessIteratorT theMiddle
    , RandomAccessIteratorT theEnd
    , boost::function<void()> theCallback
)
{
    std::inplace_merge(theBegin, theMiddle, theEnd);
    theCallback();
}

template<typename RandomAccessIteratorT> inline
void sort(
      boost::asio::io_service &theService
    , RandomAccessIteratorT theBegin
    , RandomAccessIteratorT theEnd
    , boost::function<void()> theCallback
)
{
    size_t size = theEnd - theBegin;
    
    if (size <= 4096) // block size 4k bytes
    {
        std::sort(theBegin, theEnd);
    }
    else
    {
        RandomAccessIteratorT middle = theBegin + size / 2;

        async<void(), void()>(
            boost::bind<void>(sort<RandomAccessIteratorT>, boost::ref( theService ), theBegin, middle, _1),
            boost::bind<void>(sort<RandomAccessIteratorT>, boost::ref( theService ), middle, theEnd, _1),
            boost::bind<void>(merge<RandomAccessIteratorT>, theBegin, theEnd, theCallback)
        );
    }
}
