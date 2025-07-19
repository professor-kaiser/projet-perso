#include "../../include/Layer/Layer.hpp"
#include "../../include/Activation.hpp"
#include "../../include/math/Utils.hpp"
#include <random>
#include <omp.h>

namespace Infinity
{
namespace Layer
{
    Layer::Layer(int size, Infinity::Activation::Type type, Infinity::Neuron::Type neuronType)
    : activation(type)
    {
        for(int i {0}; i < size; i++)
        {
            double biais = Activation::random(-1.0, 1.0);

            switch (neuronType)
            {
            case Infinity::Neuron::Type::DEFAULT:
                neurons.emplace_back(std::make_shared<Infinity::Neuron::Neuron>(biais, Activation::choose(type)));
                break;
            
            case Infinity::Neuron::Type::SUPERVISED:
                neurons.emplace_back(std::make_shared<Infinity::Neuron::SupervisedNeuron>(biais, Activation::choose(type)));
                break;
            default:
                break;
            }
        }
    }

    void
    Layer::connect(const std::shared_ptr<Layer>& layer)
    {
        for(const auto& neuron : this->neurons)
            neuron->connect(layer->neurons);

        this->previous = layer;
        layer->next = this->shared_from_this();
    }

    void
    Layer::forward() const
    {
        #pragma omp parallel for
        for(int i = 0; i < this->neurons.size(); i++)
            if(!this->previous.expired()) neurons[i]->activate();
    }

    void 
    Layer::backward() const
    {
        if(this->next.expired()) return;

        #pragma omp parallel for
        for(int i = 0; i < this->neurons.size(); i++)
        {
            double sum {0};
            for(const auto& nextNeuron : neurons[i]->getNexts())
                sum += nextNeuron->getDelta() * nextNeuron->getWeight(neurons[i]);

            neurons[i]->setDelta(sum * Math::derivative(neurons[i]->getActivation(), neurons[i]->getOutput()));
        }
    }

    void 
    Layer::update(double rate, bool useMiniBatch)
    {
        #pragma omp parallel for
        for (int i = 0; i < neurons.size(); i++)
        {
            neurons[i]->updateWeights(rate, useMiniBatch);
        }
    }

    void 
    Layer::init(std::vector<double> inputs)
    {
        for(int i = 0; i < this->neurons.size(); i++)
            this->neurons[i]->activate(inputs[i]);
    }

    void 
    Layer::zeroGradients()
    {
        for(const auto& neuron : this->neurons)
            neuron->zeroGradients();
    }

    void 
    Layer::accumulateGradients()
    {
        for(const auto& neuron : this->neurons)
            neuron->accumulateGradients();
    }

    json 
    Layer::serialize()
    {
        json arr = json::array();
        for(const auto& neuron : this->neurons)
        {
            arr.push_back(neuron->serialize());
        }

        return json {{"layer", arr}};
    }

    void 
    Layer::load(json data)
    {
        for(int i {0}; i < this->neurons.size(); i++)
        {
            this->neurons[i]->load(data["layer"][i]);
        }
    }

    std::shared_ptr<Infinity::Neuron::Neuron> 
    Layer::getNeuron(int index)
    {
        return this->neurons[index];
    }

    const std::weak_ptr<Layer>&
    Layer::getNext()
    {
        return this->next;
    }

    const std::weak_ptr<Layer>& 
    Layer::getPrevious()
    {
        return this->previous;
    }

    std::vector<std::shared_ptr<Infinity::Neuron::Neuron>> 
    Layer::getNeurons()
    {
        return this->neurons;
    }

    std::shared_ptr<Layer> 
    operator>>(const std::shared_ptr<Layer>& lhs, const std::shared_ptr<Layer>& rhs) 
    {
        rhs->connect(lhs);
        return rhs;
    }

    std::weak_ptr<Layer> 
    begin(const std::shared_ptr<Layer>& layer)
    {
        std::weak_ptr<Layer> it = layer;
        while(!it.lock()->getPrevious().expired())
        {
            it = it.lock()->getPrevious();
        }

        return it;
    }

    std::weak_ptr<Layer> 
    end(const std::shared_ptr<Layer>& layer)
    {
        std::weak_ptr<Layer> it = layer;
        while(!it.lock()->getNext().expired())
        {
            it = it.lock()->getNext();
        }

        return it;
    }
} // namespace Layer
} // namespace Infinity
