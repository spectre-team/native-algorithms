/*
 * SplitterSegmentExtractor.h
 * Extracts the splitter segment from the signal based on the provided
 * clear peak.
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

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Extracts the splitter segment from the signal based on the provided
/// clear peak.
/// </summary>
class SplitterSegmentExtractorPar
{
public:
    /// <summary>
    /// Extract the Splitter-Segment from the signal.
    /// </summary>
    /// <param name="peaks">List of peaks in the signal.</param>
    /// <param name="spectrum">Mzs and intensities of the signal</param>
    /// <param name="valleyIndices">Indices of the valleys w.r.t. spectrum.</param>
    /// <param name="indexOfSplittingPeak">Index of splitting peak.</param>
    /// <param name="resolutionCoeff">Resolution coefficient.</param>
    /// <returns>Indices of clear peaks in the signal.</returns>
    Spectrum ExtractSplitterSegment(const Peaks& peaks, SpectrumView spectrum,
        IndicesView valleyIndices, Index indexOfSplittingPeak,
        DataType resolutionCoeff) const;
};
}