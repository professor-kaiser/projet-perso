#include "../../include/Neuron/Neuron.hpp"
#include "../../include/Activation.hpp"
#include "../../include/Neuron/Neuron.hpp"

namespace Infinity
{
namespace Neuron
{
    Neuron::Neuron(double b, std::function<double (double)> act) 
    : bias(b), gradBias(0.0), activation(act), active(false)
    {}

    double 
    Neuron::activate()
    {
        double sum { this->bias };

        if(!this->previous.empty())
            for(const auto& [neuron, weight] : this->previous)
                sum += weight * neuron->output;

        this->output = this->activation(sum);
        this->active = true;

        return this->output;
    }

    double 
    Neuron::activate(double input)
    {
        this->output = input;
        this->active = true;

        return this->output;
    }

    void 
    Neuron::connect(std::shared_ptr<Neuron> neuron)
    {
        double weight = Activation::random(-1.0, 1.0);

        this->previous.push_back({neuron, weight});
        this->gradPrevious.push_back({neuron, 0.0});
        neuron->nexts.push_back(shared_from_this());
    }

    void 
    Neuron::connect(std::vector<std::shared_ptr<Neuron>> neurons)
    {
        for(const auto& neuron : neurons)
            this->connect(neuron);
    }

    void 
    Neuron::zeroGradients()
    {
        for(auto& gp : this->gradPrevious)
        {
            gp.second = 0.0;
        }

        this->gradBias = 0.0;
    }

    void 
    Neuron::accumulateGradients()
    {
        if(this->previous.empty()) return;
        
        for (auto& pair : this->previous)
        {
            pair.second += delta * pair.first->getOutput();
        }

        this->gradBias += delta;
    }

    void 
    Neuron::updateWeights(double rate, bool useMiniBatch)
    {
        for (int i = 0; i < this->previous.size(); i++)
        {
            double mult = (useMiniBatch ? this->gradPrevious[i].second : this->delta * this->previous[i].first->getOutput());
            this->previous[i].second -= rate * mult;
        }

        bias -= rate * this->delta;
    }

    json 
    Neuron::serialize()
    {
        return json {
            {"weights", this->getWeights()}, 
            {"bias", this->bias}
        };
    }

    void 
    Neuron::load(json data)
    {
        for(int i {0}; i < previous.size(); i++)
        {
            previous[i].second = data["weights"][i];
        }

        this->bias = data["bias"];
    }

    double 
    Neuron::getOutput()
    {
        return this->output;
    }

    void
    Neuron::setOutput(double out)
    {
        this->output = out;
    }

    double 
    Neuron::getDelta()
    {
        return this->delta;
    }

    void 
    Neuron::setDelta(double d)
    {
        this->delta = d;
    }

    std::vector<std::shared_ptr<Neuron>> 
    Neuron::getNexts()
    {
        return this->nexts;
    }

    double 
    Neuron::getWeight(const std::shared_ptr<Neuron>& neuron)
    {
        auto it = std::find_if(this->previous.begin(), this->previous.end(),
            [&neuron](const std::pair<std::shared_ptr<Neuron>, double>& pair){
                return pair.first == neuron;
            });

        return (it != this->previous.end() ? it->second : 0.0);
    }

    std::vector<double> 
    Neuron::getWeights()
    {
        std::vector<double> weights;
        for(const auto& [neuron, weight] : this->previous)
        {
            weights.push_back(weight);
        }

        return weights;
    }

    std::function<double (double)> 
    Neuron::getActivation()
    {
        return this->activation;
    }

    bool 
    Neuron::isActive()
    {
        return active;
    }
}
} // namespace Infinity
