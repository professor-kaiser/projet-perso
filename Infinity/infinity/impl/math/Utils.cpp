#include "../../include/math/Utils.hpp"

namespace Infinity
{
namespace Math
{
    double derivative(std::function<double (double)> f, double x, Derivative type, double h)
    {
        double fp {0.0};

        switch (type)
        {
        case Derivative::FORWARD:
            fp = (f(x + h) - f(x)) / h;
            break;
        
        case Derivative::BACKWARD:
            fp = (f(x) - f(x - h)) / h;
            break;

        case Derivative::CENTRAL:
            fp = (f(x + h) - f(x - h)) / (2 * h);
            break;

        default:
            break;
        }

        return fp;
    }
} // namespace Math  
} // namespace Infinity
