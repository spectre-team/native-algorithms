/*
 * DaubechiesFiltersDenoiser.cpp
 * Denoises the signal using undecimated wavelet transform.
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
#include "DaubechiesFiltersDenoiser.h"
#include "SoftThresholder.h"

namespace spectre::algorithm::wavelet
{
DaubechiesFiltersDenoiser::DaubechiesFiltersDenoiser()
{
}

static inline void ExtractCoefficientsForNoiseEstimation(Signal& highFrequencyCoeffs,
    WaveletCoefficients& coefficients)
{
    CoefficientList& noiseEstimationCoefficients = coefficients.data[0][0];
    auto chunkStart = noiseEstimationCoefficients.begin();
    auto chunkEnd = chunkStart + highFrequencyCoeffs.size();
    highFrequencyCoeffs.assign(chunkStart, chunkEnd);
}

static inline WaveletCoefficients TresholdSignal(const NoiseEstimator& noiseEstimator,
    WaveletCoefficients& coefficients, size_t signalLength)
{
    Signal highFreqCoefficients(signalLength);
    ExtractCoefficientsForNoiseEstimation(highFreqCoefficients, coefficients);
    DataType noiseTreshold = noiseEstimator.Estimate(highFreqCoefficients);
    SoftThresholder tresholder(noiseTreshold);
    return tresholder(std::move(coefficients));
}

static inline WaveletCoefficients Decompose(const WaveletDecomposerRef& decomposer,
    Signal& signal)
{
    return decomposer.Decompose(std::move(signal));
}

static inline Signal Reconstruct(const WaveletReconstructorRef& reconstructor,
    WaveletCoefficients& coefficients, size_t signalLength)
{
    return reconstructor.Reconstruct(std::move(coefficients), signalLength);
}

Signal DaubechiesFiltersDenoiser::Denoise(Signal& signal) const
{
    const size_t signalLength = signal.size();

    if (signalLength < 2)
        return signal;

    WaveletCoefficients coefficients = Decompose(m_Decomposer, signal);
    coefficients = TresholdSignal(m_NoiseEstimator, coefficients, signalLength);
    Signal denoisedSignal = Reconstruct(m_Reconstructor, coefficients, signalLength);

    return denoisedSignal;
}
}