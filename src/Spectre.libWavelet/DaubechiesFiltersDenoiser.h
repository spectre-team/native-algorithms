/*
 * DaubechiesFiltersDenoiser.h
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
#pragma once
#include <memory>
#include "DataTypes.h"
#include "Spectre.libWavelet/MedianAbsoluteDeviationNoiseEstimator.h"
#include "Spectre.libWavelet/WaveletDecomposerRef.h"
#include "Spectre.libWavelet/WaveletReconstructorRef.h"

namespace spectre::algorithm::wavelet
{
using NoiseEstimator = MedianAbsoluteDeviationNoiseEstimator;

class DaubechiesFiltersDenoiser
{
public:
    explicit DaubechiesFiltersDenoiser();
    Data Denoise(Data& signal) const;
private:
    static constexpr unsigned m_Base = 4;
    static constexpr unsigned m_LevelsOfDecomposition = 10;
    const NoiseEstimator m_NoiseEstimator;
    const WaveletDecomposerRef m_Decomposer;
    const WaveletReconstructorRef m_Reconstructor;
};
}
