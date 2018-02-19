/*
 * WaveletReconstructorRef.cpp
 * Reference implementation of wavelet reconstructor.
 *
 * A detailed explanation, can be found at
 * http://ieeexplore.ieee.org/abstract/document/4060954/
 * Page 2, equation (2) describes the reconstruction, which
 * boils down to application of reconstruction filters on 
 * high and low frequencies coefficients and computing an
 * arithmetic mean of resulting signals.
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
#include "PrecomputedDaubechiesCoefficients.h"
#include "WaveletReconstructorRef.h"
#include "WaveletUtils.h"

namespace spectre::algorithm::wavelet
{
// Average both signals, starting from startIndex and return.
static inline Signal AverageSignals(Signal& signalOne, Signal& signalTwo, unsigned startIndex)
{
    const size_t length = signalOne.size();
    Signal result(length - startIndex);
    for (unsigned i = startIndex; i < length; i++)
    {
        result[i - startIndex] = (signalOne[i] + signalTwo[i]) * 0.5;
    }
    return result;
}

// Copy part of a signal, determined by interval [0;length).
static inline void CopySelectively(Signal& source, Signal& destination, size_t length)
{
    memcpy(&destination[0], &source[0], length * sizeof(DataType));
}

// This can be approximated by current block length * 2.
static inline size_t ComputeCoefficientsNumber(unsigned level, size_t signalLength)
{
    constexpr size_t MAX_SCALE = 1 << WAVELET_LEVELS;
    const size_t numerator = (MAX_SCALE - 1) * BASIS_LENGTH + signalLength;
    const size_t denominator = MAX_SCALE;
    const size_t coeffsNumber =
        CeiledDivision(numerator, denominator) * ComputeScale(WAVELET_LEVELS - level);

    return coeffsNumber;
}

// Merges the scales together, decreasing amount of coefficient lists.
static inline void DecreaseScale(CoefficientsPerLevel& coefficients, unsigned level, size_t coeffsNumber)
{
    const unsigned shift = (1 << level) >> 1;
    for (unsigned i = 0; i < shift; i++)
    {
        const unsigned oldSize = (unsigned)coefficients[i].size();
        coefficients[i].resize(coeffsNumber);
        unsigned oldRowColumn = oldSize - 1;
        unsigned newRowColumn = oldSize - 1;
        const unsigned startingIndex = oldSize * 2 - 1;
        for (int j = startingIndex; j >= 0; j--)
        {
            unsigned row = j % 2 ? shift + i : i; // TODO check if this is converted to div and unroll if it is
            unsigned column = j % 2 ? newRowColumn-- : oldRowColumn--;
            coefficients[i][j] = coefficients[row][column];
        }
    }
    coefficients.resize(shift);
}

// Apply reconstruction filters at all scales of certain level.
inline void WaveletReconstructorRef::ApplyFilters(WaveletCoefficients& coefficients,
    size_t scale, size_t blockLength, unsigned level) const
{
    using namespace precomputed;
    constexpr unsigned LOW_FREQ_INDEX = WAVELET_LEVELS;
    auto& coeffs = coefficients.data;
    for (unsigned i = 0; i < scale; i++)
    {
        Signal lowFreqCoefficients =
            m_Convolution.Convolve(ReconstructionLowPassFilter, coeffs[LOW_FREQ_INDEX][i], blockLength);
        Signal highFreqCoefficients =
            m_Convolution.Convolve(ReconstructionHighPassFilter, coeffs[level][i]);
        Signal averagedCoefficients =
            AverageSignals(lowFreqCoefficients, highFreqCoefficients, BASIS_LENGTH);
        CopySelectively(averagedCoefficients, coeffs[LOW_FREQ_INDEX][i], blockLength - BASIS_LENGTH);
    }
}

WaveletReconstructorRef::WaveletReconstructorRef()
{
}

Signal WaveletReconstructorRef::Reconstruct(WaveletCoefficients&& coefficients, size_t signalLength) const
{
    constexpr unsigned LOW_FREQ_INDEX = WAVELET_LEVELS;
    auto& coeffs = coefficients.data;
    size_t scale = ComputeScale(WAVELET_LEVELS - 2);
    size_t blockLength = ComputeBlockLength(WAVELET_LEVELS - 1, scale, signalLength);
    size_t coeffsNum = 0;

    for (unsigned level = WAVELET_LEVELS - 1; level > 0; level--)
    {
        ApplyFilters(coefficients, scale, blockLength, level);
        scale = ComputeScale(level - 2);
        blockLength = ComputeBlockLength(level - 1, scale, signalLength);
        coeffsNum = ComputeCoefficientsNumber(level, signalLength);
        DecreaseScale(coeffs[LOW_FREQ_INDEX], level, coeffsNum);
    }
    ApplyFilters(coefficients, scale, blockLength, 0);
    coeffs[LOW_FREQ_INDEX][0].resize(signalLength);

    return coeffs[LOW_FREQ_INDEX][0];
}
}