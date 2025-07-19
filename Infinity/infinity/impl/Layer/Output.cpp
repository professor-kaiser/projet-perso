#include "../../include/Layer/Output.hpp"
#include "../../include/math/Utils.hpp"
#include "../../include/Neuron/SupervisedNeuron.hpp"
#include <cmath>

namespace Infinity
{
namespace Layer
{
    Output::Output(int size, Infinity::Activation::Type type)
    : Layer(size, type, Infinity::Neuron::Type::SUPERVISED)
    {}

    void 
    Output::forward() const
    {
        Layer::forward();
        if(this->activation == Infinity::Activation::Type::SOFTMAX)
        {
            auto maxIt = std::max_element(this->neurons.begin(), this->neurons.end(),
                [](const std::shared_ptr<Infinity::Neuron::Neuron>& na, const std::shared_ptr<Infinity::Neuron::Neuron>& nb)
                {
                    return na->getOutput() < nb->getOutput();
                });

            double sum {0.0};

            for(const auto& neuron : this->neurons)
            {
                sum += std::exp(neuron->getOutput() - (*maxIt)->getOutput());
            }

            for(const auto& neuron : this->neurons)
            {
                neuron->setOutput(std::exp(neuron->getOutput() - (*maxIt)->getOutput()) / sum);
            }
        }
    }

    void
    Output::backward() const
    {
        for(const auto& neuron : this->neurons)
        {
            using namespace Infinity;

            std::shared_ptr<Neuron::SupervisedNeuron> supNeuron = std::dynamic_pointer_cast<Neuron::SupervisedNeuron>(neuron);

            double prediction = (supNeuron->isActive() ? supNeuron->getOutput() : supNeuron->activate());
            double target = supNeuron->getTarget();
            double error = prediction - target;

            double deriv {(this->activation == Infinity::Activation::Type::SOFTMAX) ? 1.0 : Math::derivative(neuron->getActivation(), prediction)};
            supNeuron->setDelta(error * deriv);
        }
    }

    void 
    Output::updateTarget(std::vector<double> y)
    {
        auto neurons = this->getNeurons();
        for(int i {0}; i < y.size(); i++)
        {
            std::shared_ptr<Neuron::SupervisedNeuron> neuron = std::dynamic_pointer_cast<Neuron::SupervisedNeuron>(neurons[i]);
            neuron->setTarget(y[i]);
        }
    }
} // namespace Layer
} // namespace Infinity
