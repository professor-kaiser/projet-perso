#ifndef __INFINITY_NEURAL_NETWORK__
#define __INFINITY_NEURAL_NETWORK__

#include <list>
#include <memory>
#include <nlohmann/json.hpp>
#include <fstream>
#include "Layer/Layer.hpp"

using json = nlohmann::json;

namespace Infinity
{
    class NeuralNetwork
    {
    private:
        std::weak_ptr<Layer::Layer> linput;
        std::weak_ptr<Layer::Layer> loutput;

    public:
        using Sample = std::vector<double>;
        using Dataset = std::vector<std::pair<Sample, Sample>>;

        NeuralNetwork(const std::shared_ptr<Layer::Layer>& layer);
        NeuralNetwork(const std::shared_ptr<Layer::Layer>& input, const std::shared_ptr<Layer::Layer>& output);
        void train(Dataset dataset, int epochs, double rate = 0.01);  
        void train(Dataset dataset, int epochs, double rate, int batch_size, const std::string& savedata);
        void forward() const;
        void backward() const;
        void zeroGradients();
        void accumulateGradients();
        void update(double rate = 0.01, bool useMiniBatch = false) const;
        json serialize();
        json loadJSON(std::string filename);
        void load(std::string filename);
        void save(std::string filename);
    };
} // namespace Infinity

#endif