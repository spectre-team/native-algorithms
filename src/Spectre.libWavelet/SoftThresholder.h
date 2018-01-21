/*
 * SoftThresholder.h
 * Performs a soft tresholding on the provided signal.
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
/// Applies soft tresholding on the signal provided.
/// </summary>
class SoftThresholder
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="SoftThresholder"/> class.
    /// </summary>
    /// <param name="threshold">Treshold to be used.</param>
    explicit SoftThresholder(DataType threshold);
    /// <summary>
    /// Tresholds the signal
    /// </summary>
    /// <param name="signal">Signal to apply tresholding to.</param>
    /// <returns>Tresholded signal.</returns>
    Signal operator()(const Signal& signal) const;
private:
    const DataType m_Threshold;
};
}
