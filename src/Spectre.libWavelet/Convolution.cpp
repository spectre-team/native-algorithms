#include "Convolution.h"

namespace spectre::algorithm::wavelet
{
Convolution::Convolution()
{
}

Signal Convolution::Filter(const Signal &signal)
{
    std::vector<DataType> filtered(signal.size());
    for(unsigned n = 0u; n < signal.size(); ++n)
    {
        for (unsigned i = 0u; i < filtered.size() && static_cast<int>(n) - static_cast<int>(i) >= 0; ++i)
        {
            filtered[n] += signal[n - i];
        }
    }
    return filtered;
}
}
