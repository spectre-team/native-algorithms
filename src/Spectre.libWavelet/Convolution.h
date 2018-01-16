#pragma once
#include <span.h>
#include "DataTypes.h"

namespace spectre::algorithm::wavelet
{
class Convolution
{
public:
    explicit Convolution();
    Signal Filter(const Signal& signal);
};
}
