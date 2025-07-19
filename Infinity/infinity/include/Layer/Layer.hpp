#ifndef __INFINITY_LAYER__
#define __INFINITY_LAYER__

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Neuron/Neuron.hpp"
#include "../Neuron/SupervisedNeuron.hpp"
#include "../Activation.hpp"

using json = nlohmann::json;

namespace Infinity
{
namespace Layer
{
    class Layer : public std::enable_shared_from_this<Layer>
    {
    protected:
        std::vector<std::shared_ptr<Infinity::Neuron::Neuron>> neurons;
        std::weak_ptr<Layer> previous;
        std::weak_ptr<Layer> next;
        const Infinity::Activation::Type activation;

    public:
        Layer(
            int size, 
            Infinity::Activation::Type type, 
            Infinity::Neuron::Type neuronType = Infinity::Neuron::Type::DEFAULT);
        void connect(const std::shared_ptr<Layer>& layer);
        virtual void forward() const;
        virtual void backward() const;
        virtual void update(double rate = 0.01, bool useMiniBatch = false);
        void init(std::vector<double> inputs);
        void zeroGradients();
        void accumulateGradients();
        json serialize();
        void load(json data);

        std::shared_ptr<Infinity::Neuron::Neuron> getNeuron(int index);
        const std::weak_ptr<Layer>& getNext();
        const std::weak_ptr<Layer>& getPrevious();
        std::vector<std::shared_ptr<Infinity::Neuron::Neuron>> getNeurons();
    };

    std::shared_ptr<Layer> operator>>(
        const std::shared_ptr<Layer>& lhs, 
        const std::shared_ptr<Layer>& rhs);

    std::weak_ptr<Layer> begin(const std::shared_ptr<Layer>& layer);
    std::weak_ptr<Layer> end(const std::shared_ptr<Layer>& layer);
} // namespace Layer
} // namespace Infinity

#endif