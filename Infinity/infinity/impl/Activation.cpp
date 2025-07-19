#include "../include/Activation.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace Infinity
{
namespace Activation
{
    double relu(double x)
    {
        return std::max(0.0, x);
    }

    double sigmoid(double x)
    {
        return 1 / (1 + std::exp(-x));
    }

    double tanh(double x)
    {
        return std::tanh(x);
    }

    double linear(double x)
    {
        return x;
    }

    std::function<double (double)> choose(Type type)
    {
        std::function<double (double)> activation = relu;

        switch (type)
        {
        case Type::RELU: 
            activation = relu; 
            break;

        case Type::SIGMOID: 
            activation = sigmoid;
            break;

        case Type::TANH:
            activation = tanh;
            break;

        case Type::LINEAR:
            activation = linear;
            break;

        case Type::SOFTMAX:
            activation = linear;
            break;
        }

        return activation;
    }

    double random(double min, double max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(min, max);
        
        return dist(gen);
    }
} // namespcae Activation
} // namespace Infinity
