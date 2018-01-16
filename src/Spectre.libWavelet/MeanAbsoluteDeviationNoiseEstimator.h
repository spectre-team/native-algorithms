#pragma once
#include "DataTypes.h"

namespace spectre::algorithm::wavelet
{
class MeanAbsoluteDeviationNoiseEstimator
{
public:
    explicit MeanAbsoluteDeviationNoiseEstimator(DataType multiplier=1.0);
    DataType Estimate(const Signal& intensities) const;
private:
    const DataType m_Multiplier;
};
}
