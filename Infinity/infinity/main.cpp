#include "include/Layer/Layer.hpp"
#include "include/Layer/Output.hpp"
#include "include/Layer/Input.hpp"
#include "include/NeuralNetwork.hpp"
#include "data/loader.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <filesystem>

using namespace Infinity;
// g++ -fopenmp -std=c++23 main.cpp impl/Activation.cpp impl/Neuron/*.cpp impl/math/*.cpp impl/Layer/*.cpp impl/NeuralNetwork.cpp -o main

int main()
{
    std::shared_ptr<Layer::Layer> input = std::make_shared<Layer::Input>(784, Activation::Type::LINEAR);
    std::shared_ptr<Layer::Layer> hidden1 = std::make_shared<Layer::Layer>(128, Activation::Type::TANH);
    std::shared_ptr<Layer::Layer> hidden2 = std::make_shared<Layer::Layer>(64, Activation::Type::TANH);
    std::shared_ptr<Layer::Layer> output = std::make_shared<Layer::Output>(10, Activation::Type::SOFTMAX);

    input >> hidden1 >> hidden2 >> output;

    std::shared_ptr<NeuralNetwork> neuralNetwork = std::make_shared<NeuralNetwork>(input);
    int epochs {20};
    double rate {0.01};
    int batch {128};
    const std::string savedata("28x28_train.json");

    std::cout << "[B] Loading json" << std::endl;
    NeuralNetwork::Dataset dataset = loadDataset("data/data_minified.json");
    std::cout << "[E] Loading json" << std::endl;

    neuralNetwork->train(dataset, epochs, rate, batch, savedata);

    return 0;
}