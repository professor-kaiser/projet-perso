#ifndef __INFINITY_NEURON_SUPERVISEDNEURON__
#define __INFINITY_NEURON_SUPERVISEDNEURON__

#include <functional>
#include "Neuron.hpp"

namespace Infinity
{
namespace Neuron
{
    class SupervisedNeuron : public Neuron
    {
    private:
        double target;

    public:
        SupervisedNeuron(double b, std::function<double (double)> act);
        double getTarget();
        void setTarget(double t);
    };
    
} // namespace Neuron
} // namespace Infinity

#endif