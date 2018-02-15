/*
* WaveletUtils.h
* Common functions used by wavelet decomposition and reconstruction.
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
#include "AlgorithmConstants.h"

namespace spectre::algorithm::wavelet
{
/// <summary>
/// Computes the current scale of the wavelet.
/// </summary>
/// <param name="level">Wavelet level.</param>
/// <returns>Scale.</returns>
inline size_t ComputeScale(unsigned level)
{
    return 1ULL << (level + 1); // 2^(L + 1)
}

/// <summary>
/// Performs a ceiled divison on integers. No error checking against divisor = 0.
/// </summary>
/// <param name="dividend">The nominator.</param>
/// <param name="divisor">The denominator.</param>
/// <returns>Result of ceil(dividend/divisor).</returns>
inline size_t CeiledDivision(size_t dividend, size_t divisor)
{
    return dividend / divisor + (dividend % divisor != 0);
}

/// <summary>
/// Computes a number of samples to be filtered by wavelet filters.
/// </summary>
/// <param name="level">Given wavelet level.</param>
/// <param name="scale">Given wavelet scale.</param>
/// <param name="signalLength">Current length of the signal.</param>
/// <returns>Number of samples to be filtered.</returns>
inline size_t ComputeBlockLength(unsigned level, size_t scale, size_t signalLength)
{
    constexpr unsigned BASIS_LENGTH = (WAVELET_BASIS * 2) - 1;

    const size_t numerator = signalLength + ((1 << level) - 1) * BASIS_LENGTH;
    const size_t denominator = scale;
    const size_t blockLength = CeiledDivision(numerator, denominator) + BASIS_LENGTH;

    return blockLength;
}
}