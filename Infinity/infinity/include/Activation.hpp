#ifndef __INFINITY_ACTIVATION__
#define __INFINITY_ACTIVATION__

#include <functional>

namespace Infinity
{
namespace Activation
{
    enum class Type
    {
        RELU,
        SIGMOID,
        TANH,
        LINEAR,
        SOFTMAX
    };

    double relu(double x);
    double sigmoid(double x);
    double tanh(double x);
    double linear(double x);

    std::function<double (double)> choose(Type type);
    double random(double min, double max);
} // namespace Activation
} // namespace Infinity

#endif