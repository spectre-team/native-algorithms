/*
 * MeanAbsoluteDeviationNoiseEstimator.h
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
#pragma once
#include "DataTypes.h"

namespace spectre::algorithm::wavelet
{
/// <summary>
/// Estimates the Mean-absolute devaition of the noise in the signal
/// </summary>
class MeanAbsoluteDeviationNoiseEstimator
{
public:
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="MeanAbsoluteDeviationNoiseEstimator"/> class.
    /// </summary>
    /// <param name="multiplier">The multiplier used for computations.</param>
    explicit MeanAbsoluteDeviationNoiseEstimator(DataType multiplier=1.0);
    /// <summary>
    /// Estimates the MAD of the noise.
    /// </summary>
    /// <param name="intensities">Signal to be analyzed.</param>
    /// <returns>Estiamte of the noise in the signal.</returns>
    DataType Estimate(const Signal& intensities) const;
private:
    const DataType m_Multiplier;
};
}
