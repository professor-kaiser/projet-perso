#ifndef __INFINITY_NEURON__
#define __INFINITY_NEURON__

#include <vector>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include "INeuron.hpp"

using json = nlohmann::json;

namespace Infinity
{
namespace Neuron
{
    enum class Type
    {
        DEFAULT,
        SUPERVISED
    };

    class Neuron : public std::enable_shared_from_this<Neuron>
    {
    private:
        using connections = std::vector<std::pair<std::shared_ptr<Neuron>, double>>;

        connections previous;
        connections gradPrevious;
        std::vector<std::shared_ptr<Neuron>> nexts;
        double bias;
        double gradBias;
        double output;
        double delta;
        std::function<double (double)> activation;
        bool active;

    public:
        Neuron(double b, std::function<double (double)> act);
        virtual ~Neuron() = default;
        double activate();
        double activate(double input);
        void connect(std::shared_ptr<Neuron> neuron);
        void connect(std::vector<std::shared_ptr<Neuron>> neurons);
        void zeroGradients();
        void accumulateGradients();
        void updateWeights(double rate = 0.01, bool useMiniBatch = false);
        json serialize();
        void load(json data);

        double getOutput();
        void setOutput(double out);
        double getDelta();
        void setDelta(double delta);
        std::vector<std::shared_ptr<Neuron>> getNexts();
        double getWeight(const std::shared_ptr<Neuron>& neuron);
        std::vector<double> getWeights();
        std::function<double (double)> getActivation();
        bool isActive();
    };
}
} // namespace Infinity


#endif