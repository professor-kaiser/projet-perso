#ifndef __INFINITY_LAYER_INPUT__
#define __INFINITY_LAYER_INPUT__

#include "Layer.hpp"

namespace Infinity
{
namespace Layer
{
    class Input final : public Layer
    {
    public:
        Input(int size, Infinity::Activation::Type type);
        void forward() const override;
        void backward() const override;
        void update(double rate = 0.01, bool useMiniBatch = false) override;
    };
} // namespace Layer
} // namespace Infinity

#endif