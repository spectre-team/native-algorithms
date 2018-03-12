/*
* WaveletDecomposerRef.cpp
* Reference implementation of wavelet decomposer.
*
* Bibliography:
* https://en.wikipedia.org/wiki/Stationary_wavelet_transform#Implementation
* http://web.iitd.ac.in/~sumeet/WaveletTutorial.pdf
* http://www.egr.msu.edu/~aviyente/ece802swt.ppt
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
#include "WaveletDecomposerRef.h"
#include "WaveletUtils.h"

namespace spectre::algorithm::wavelet
{
// The function performs an 'intelligent' downsampling, where every second coefficient is
// stored as a coefficient of 'new' sample, instead of just being discarded. This allows
// the decomposition to retain more information about the signal.
static inline void DownsampleByDissolving(CoefficientsPerLevel& coefficients, unsigned level, unsigned blockLength)
{
    const unsigned shift = (1 << level) >> 1;
    for (unsigned i = 0; i < shift; i++)
    {
        coefficients[shift + i].resize(blockLength);
        unsigned oldRowColumn = 0;
        unsigned newRowColumn = 0;
        for (unsigned j = 0; j < coefficients[i].size(); j++)
        {
            unsigned row = j % 2 ? shift + i : i;
            unsigned column = j % 2 ? newRowColumn++ : oldRowColumn++;
            coefficients[row][column] = coefficients[i][j];
        }
        coefficients[i].resize(oldRowColumn); // Used to fill moved entries with zeros.
        coefficients[i].resize(blockLength);
    }
}

// Applies the high and low pass filters on the current low-frequency
// wavelet coefficients, at the level determined by the caller.
// https://upload.wikimedia.org/wikipedia/commons/1/16/Wavelets_-_SWT_Filter_Bank.png
// The figure above depicts the process, where h_j prepresents a high- and g_j a low-pass
// filters, applied at level j.
// All the low frequency coefficients are overridden on each call, while the high
// frequnecy ones are just added.
inline void WaveletDecomposerRef::ApplyFilters(WaveletCoefficients& coefficients,
    CoefficientsPerLevel& lowFrequencyCoefficients, size_t scale, size_t blockLength, unsigned level) const
{
    coefficients.data[level].resize(scale); // Adjust number of coefficient lists for that certain level
    for (unsigned i = 0; i < scale; i++)
    {
        lowFrequencyCoefficients[i].resize(blockLength); // Extend with zeros/shrink to match size
        coefficients.data[level][i] = // Apply Daubechies4 decomposition high pass filter
            m_Convolution.Convolve(precomputed::DecompositionHighPassFilter, lowFrequencyCoefficients[i]);
        lowFrequencyCoefficients[i] = // Apply Daubechies4 decomposition low pass filter
            m_Convolution.Convolve(precomputed::DecompositionLowPassFilter, lowFrequencyCoefficients[i]);
    }
}

WaveletDecomposerRef::WaveletDecomposerRef()
{
};

WaveletCoefficients WaveletDecomposerRef::Decompose(Data&& signal) const
{
    const size_t signalLength = signal.size();

    WaveletCoefficients coefficients;
    CoefficientsPerLevel& lowFrequencyCoefficients = coefficients.data[WAVELET_LEVELS];

    // The number of samples N used to generate N coefficient lists
    // for the lowest frequency decomposition.
    constexpr size_t LOW_FREQ_SAMPLES_NUM = 1 << (WAVELET_LEVELS - 1); // 2^(WL - 1)
    lowFrequencyCoefficients.resize(LOW_FREQ_SAMPLES_NUM);
    lowFrequencyCoefficients[0] = std::move(signal); // Initialize first line with signal

    size_t scale = 1; // Initial wavelet scale (or it's frequency)
    size_t blockLength = ComputeBlockLength(0, scale, signalLength); // Initial number of coefficients

    for (unsigned currentLevel = 0; currentLevel < WAVELET_LEVELS - 1; currentLevel++)
    {
        ApplyFilters(coefficients, lowFrequencyCoefficients, scale, blockLength, currentLevel);

        scale = ComputeScale(currentLevel);
        blockLength = ComputeBlockLength(currentLevel + 1, scale, signalLength);
        DownsampleByDissolving(lowFrequencyCoefficients, currentLevel + 1, (unsigned)blockLength);
    }

    ApplyFilters(coefficients, lowFrequencyCoefficients, scale, blockLength, WAVELET_LEVELS - 1);

    return coefficients;
}
}
