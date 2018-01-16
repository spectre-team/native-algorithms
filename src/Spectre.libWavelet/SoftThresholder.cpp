#include "Spectre.libFunctional/Transform.h"
#include "SoftThresholder.h"

namespace spectre::algorithm::wavelet
{
SoftThresholder::SoftThresholder(DataType threshold)
    : m_Threshold(threshold)
{
}

Signal SoftThresholder::operator()(const Signal &signal) const
{
    const auto& threshold = m_Threshold;
    const auto thresholding = [threshold](DataType value)
    {
        return std::max(static_cast<DataType>(0), std::abs(value) - threshold) * (value < 0 ? -1 : 1);
    };
    return core::functional::transform(gsl::as_span(signal), thresholding);
}
}
