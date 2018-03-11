/*
 * WaveletReconstructorRef.h
 * Reference implementation of wavelet reconstructor.
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
/// Reconstructs the signal from daubechies coefficients.
/// </summary>
class WaveletReconstructorRef
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="WaveletReconstructorRef"/> class.
    /// </summary>
    explicit WaveletReconstructorRef();
    /// <summary>
    /// Reconstructs the signal based on daubechies coefficients
    /// </summary>
    /// <param name="coefficients">Coefficents used to reconstruct the signal.</param>
    /// <param name="signalLength">Length of signal to be reconstructed.</param>
    /// <returns>Reconstructed signal.</returns>
    Data Reconstruct(WaveletCoefficients&& coefficients, size_t signalLength) const;

private:
    inline void WaveletReconstructorRef::ApplyFilters(
        WaveletCoefficients& coefficients, size_t scale,
        size_t blockLength, unsigned level) const;

    const Convolution m_Convolution;
};
}
