#include "../../include/Layer/Input.hpp"
#include <cmath>

namespace Infinity
{
namespace Layer
{
    Input::Input(int size, Infinity::Activation::Type type)
    : Layer(size, type, Infinity::Neuron::Type::SUPERVISED)
    {}

    void 
    Input::forward() const
    {
    }

    void
    Input::backward() const
    {
    }

    void 
    Input::update(double rate, bool useMiniBatch)
    {
    }
} // namespace Layer
} // namespace Infinity
