/*
 * MeanAbsoluteDeviationNoiseEstimator.cpp
 * Estimates mean absolute devation of noise in the signal.
 *
   Copyright 2018 Michal Gallus

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "Spectre.libStatistics/Statistics.h"
#include "MeanAbsoluteDeviationNoiseEstimator.h"

namespace spectre::algorithm::wavelet
{
MeanAbsoluteDeviationNoiseEstimator::MeanAbsoluteDeviationNoiseEstimator(DataType multiplier) :
    m_Multiplier(multiplier)
{
}

DataType MeanAbsoluteDeviationNoiseEstimator::Estimate(const Signal& intensities) const
{
    constexpr auto locationOfThirdQuartileInNormalDistribution = static_cast<DataType>(.6745);
    return m_Multiplier
        * sqrt(2 * log(intensities.size()))
        * statistics::simple_statistics::MeanAbsoluteDeviation(gsl::as_span(intensities))
        / locationOfThirdQuartileInNormalDistribution;
}
}