async
=====

 The framework for supporting a style of parallel programming
in which problems are solved by (recursively) splitting them into subtasks
that are solved in parallel, waiting for them to complete,
and then composing results.

 The fork/join framework provides a very straightforward and intuitive structure
to conquer problems.

example
=======
int sequentialFibonacci(n);

void combineFibonacci(int x, int y, function<void(int)> sendResult)
{
  sendResult(x + y);
}

void calculateFibonacci(asio::io_service& theService, int n, function<void(int)> onComplite)
{
  if (n <= threshold)
  {
    f(sequentialFibonacci(n));
  }
  else
  {
    async<void(int), void(int)>(
        io_service,
        bind(calculateFibonacci, ref(theService), n - 1, _1),
        bind(calculateFibonacci, ref(theService), n - 2, _1),
        bind(combineFibonacci, _1, _2, onComplite));
  }
}
