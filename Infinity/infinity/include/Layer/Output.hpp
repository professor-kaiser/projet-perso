#ifndef __INFINITY_LAYER_OUTPUT__
#define __INFINITY_LAYER_OUTPUT__

#include "Layer.hpp"

namespace Infinity
{
namespace Layer
{
    class Output final : public Layer
    {
    public:
        Output(int size, Infinity::Activation::Type type);
        void forward() const override;
        void backward() const override;
        void updateTarget(std::vector<double> y);
    };
} // namespace Layer
} // namespace Infinity

#endif