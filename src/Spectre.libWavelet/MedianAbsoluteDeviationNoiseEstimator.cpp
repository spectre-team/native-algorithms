/*
 * MedianAbsoluteDeviationNoiseEstimator.cpp
 * Estimates median absolute devation of noise in the signal.
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
#include "MedianAbsoluteDeviationNoiseEstimator.h"

namespace spectre::algorithm::wavelet
{
    MedianAbsoluteDeviationNoiseEstimator::MedianAbsoluteDeviationNoiseEstimator(DataType multiplier) :
    m_Multiplier(multiplier)
{
}

DataType MedianAbsoluteDeviationNoiseEstimator::Estimate(Data& highFreqCoefficients) const
{
    constexpr auto inverseOfThirdQuartileInNormalDistribution = static_cast<DataType>(1.0 / .6745);
    return m_Multiplier
        * sqrt(2 * log(highFreqCoefficients.size()))
        * statistics::simple_statistics::MedianAbsoluteDeviation(gsl::make_span(highFreqCoefficients))
        * inverseOfThirdQuartileInNormalDistribution;
}
}