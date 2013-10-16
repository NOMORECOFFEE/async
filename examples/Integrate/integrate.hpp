#ifndef INTEGRATE_HPP_INCLUDED
#define INTEGRATE_HPP_INCLUDED 1


const double EPS = 0.001;

// declaration, forward
double f(double x);


inline
double area(double l, double r)
{
    return (f(l) + f(r)) * (r - l) / 2;
}

inline
void integrate(
    boost::asio::io_service &theService,
    double l,
    double r,
    boost::function<void(double)> theCallback
)
{
    double m = (l + r) / 2;
    double la = area(l, m);
    double ra = aread(m, r); 
    if (abs(la + ra - area(l, r)) > EPS)
    {
        async<void(double), void(double)>(
            boost::ref( theService ),
            boost::bind(integrate, boost::ref( theService ), l, m, _1),
            boost::bind(integrate, boost::ref( theService ), m, r, _1),
            boost::bind(theCallback, boost::bind(std::plus<double>(), _1, _2))
        );
    }
    else
    {
        theCallback(la + ra);
    }
}
#endif // INTEGRATE_HPP_INCLUDED
