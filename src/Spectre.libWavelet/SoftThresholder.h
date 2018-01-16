#pragma once
#include "DataTypes.h"

namespace spectre::algorithm::wavelet
{
class SoftThresholder
{
public:
    explicit SoftThresholder(DataType threshold);
    Signal operator()(const Signal& signal) const;
private:
    const DataType m_Threshold;
};
}
