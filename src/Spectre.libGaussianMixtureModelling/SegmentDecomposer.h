/*
 * SegmentDecomposer.h
 * Provides implementation of Segment Decomposer algorithm
 * used for Gaussian Mixture Modelling.
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
#include "Spectre.libGaussianMixtureModelling/SignalDecomposer.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Takes a segment and returns its decomposition in a form of a gaussian
/// mixture model.
/// </summary>
/// <param name="segment">Splitter segment containing the splitter.</param>
/// <param name="resolutionCoefficient">Signal resolution.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <returns>Gaussian Mixture Model of the segment.</returns>
template<typename DynamicProgramming, typename ExpectationMaximization>
inline MixtureModel DecomposeSegment(SpectrumView segment,
    DataType resolutionCoefficient, const GmmOptions& options)
{
    constexpr int minNumOfBlocks = 1;
    const DataType penaltyCoefficient = options.segmentPenaltyCoefficient;
    const bool decomposeSplitter = false;
    return DecomposeSignal<DynamicProgramming, ExpectationMaximization>(
        segment, penaltyCoefficient, resolutionCoefficient, options,
        minNumOfBlocks, decomposeSplitter);
}
}