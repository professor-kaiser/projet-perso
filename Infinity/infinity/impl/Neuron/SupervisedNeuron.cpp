#include "../../include/Neuron/SupervisedNeuron.hpp"

namespace Infinity
{
namespace Neuron
{
    SupervisedNeuron::SupervisedNeuron(double b, std::function<double (double)> act)
    : Neuron(b, act)
    {}

    double
    SupervisedNeuron::getTarget()
    {
        return this->target;
    }

    void 
    SupervisedNeuron::setTarget(double t)
    {
        this->target = t;
    }
} // namespace Neuron
} // namespace Infinity