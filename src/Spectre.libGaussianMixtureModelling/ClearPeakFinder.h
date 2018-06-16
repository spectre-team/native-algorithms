/*
* ClearPeakFinder.h
* Finds indices of splitting peaks from among given peaks.
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
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/GmmOptions.h"
#include <vector>

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Finds indices of splitting peaks from among given peaks.
/// </summary>
class ClearPeakFinder
{
public:
    /// <summary>
    /// Finds splitting peaks in the signal.
    /// </summary>
    /// <param name="peakIndices">Signal in which minima shall be found.</param> // TODO Adjust this
    /// <param name="mzs">M/zs of the signal.</param>
    /// <param name="intensities">Intensities of the signal.</param>
    /// <param name="peakQualities">Peak qualities.</param>
    /// <returns>Indices of clear peaks in the signal.</returns>
    Indices FindClearPeaks(const Peaks& peaks, const DataView& peakQualities,
        const GmmOptions& options, DataType resolutionCoeff) const;
};
}
