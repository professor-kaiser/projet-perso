#ifndef __INFINITY_INTERFACE_NEURON__
#define __INFINITY_INTERFACE_NEURON__

#include <vector>

namespace Infinity
{
    class INeuron
    {
    public:
        virtual ~INeuron() = default;
        virtual double activate(const std::vector<double>& inputs) = 0;
    };
} // namespace Infinity

#endif