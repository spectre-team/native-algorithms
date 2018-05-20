/*
* WaveletDecomposerRef.h
* Reference implementation of wavelet decomposer.
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
#include "Convolution.h"
#include "DataTypes.h"
#include "WaveletCoefficients.h"

namespace spectre::algorithm::wavelet
{
/// <summary>
/// Decomposes the signal into set of Daubechies coefficients.
/// </summary>
class WaveletDecomposerRef
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="WaveletDecomposer"/> class.
    /// </summary>
    explicit WaveletDecomposerRef();
    /// <summary>
    /// Decomposes the signal into wavelet coefficients.
    /// </summary>
    /// <param name="signal">Signal to decompose.</param>
    /// <returns>Set of Daubechies wavelet coefficients.</returns>
    WaveletCoefficients Decompose(Data&& signal) const;

private:
    inline void ApplyFilters(
        WaveletCoefficients& coefficients,
        CoefficientsPerLevel& lowFrequencyCoefficients,
        size_t scale, size_t blockLength, unsigned level) const;

    const Convolution m_Convolution;
};
}
