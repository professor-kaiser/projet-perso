#ifndef __INFINITY_MATH_UTILS__
#define __INFINITY_MATH_UTILS__

#include <functional>

namespace Infinity
{
namespace Math
{
    enum class Derivative
    {
        FORWARD,
        BACKWARD,
        CENTRAL
    };

    double derivative(std::function<double (double)> f, double x, Derivative type = Derivative::CENTRAL, double h = 1e-5);
} // namespace Math
} // namespace Infinity


#endif