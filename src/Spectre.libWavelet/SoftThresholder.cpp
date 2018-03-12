/*
 * SoftThresholder.cpp
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
#include "Spectre.libFunctional/Transform.h"
#include "SoftThresholder.h"

namespace spectre::algorithm::wavelet
{
SoftThresholder::SoftThresholder(DataType threshold)
    : m_Threshold(threshold)
{
}

WaveletCoefficients SoftThresholder::operator()(WaveletCoefficients&& coefficients) const
{
    auto& coeffs = coefficients.data;
    for (unsigned level = 0; level < WAVELET_LEVELS + 1; level++)
    {
        for (unsigned scale = 0; scale < coeffs[level].size(); scale++)
        {
            for (unsigned i = 0; i < coeffs[level][scale].size(); i++)
            {
                DataType value = coeffs[level][scale][i];
                coeffs[level][scale][i] = std::max(0.0, std::abs(value) - m_Threshold);
                coeffs[level][scale][i] = std::copysign(coeffs[level][scale][i], value);
            }
        }
    }

    return coefficients;
}
}
