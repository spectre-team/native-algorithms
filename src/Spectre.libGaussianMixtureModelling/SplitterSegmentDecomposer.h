/*
* SplitterSegmentDecomposer.h
* Provides implementation of Splitter Segment Decomposer algorithm
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
#include <cmath>
#include "Spectre.libGaussianMixtureModelling/SignalDecomposer.h"
#include "Spectre.libFunctional/Filter.h"

namespace spectre::unsupervised::gmm
{
static MixtureModel ExtractSplitter(MixtureModel&, DataType);

/// <summary>
/// Takes a splitter segment and returns a splitter in a form of a gaussian
/// mixture model.
/// </summary>
/// <param name="segment">Splitter segment containing the splitter.</param>
/// <param name="splittingPeakMz">Mz location of a splitter.</param>
/// <param name="resolutionCoefficient">Signal resolution.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <returns>Matrix of qualities as described in the summary.</returns>
inline MixtureModel DecomposeSplitterSegment(SpectrumView segment,
    DataType splittingPeakMz, DataType resolutionCoefficient,
    GmmOptions& options)
{
    constexpr int minNumOfBlocks = 2;
    const DataType penaltyCoefficient = options.splittingPenaltyCoefficient;
    MixtureModel components =  DecomposeSignal(segment, penaltyCoefficient,
        resolutionCoefficient, options, minNumOfBlocks, true);
    return ExtractSplitter(components, splittingPeakMz);
}

static Data ComputeComponentQualities(const MixtureModel& components,
    DataType splittingPeakMz)
{
    const unsigned componentsNum = (unsigned)components.size();
    Data gaussianQualities(componentsNum);
    for (unsigned i = 0; i < componentsNum; i++)
    {
        const DataType distance = components[i].mean - splittingPeakMz;
        const DataType scaledDist = distance / components[i].deviation;
        const DataType module = std::abs(scaledDist);
        gaussianQualities[i] = module;
    }

    return gaussianQualities;
}

static Indices FindSplitterIndices(const Data& qualities)
{
    constexpr DataType qualityThreshold = 3.0; // I know, right.
    const unsigned componentsNum = (unsigned)qualities.size();
    Indices splitterIndices;
    splitterIndices.reserve(componentsNum);

    for (unsigned i = 0; i < componentsNum; i++)
    {
        if (qualities[i] <= qualityThreshold)
            splitterIndices.push_back(i);
    }

    if (splitterIndices.empty())
    {
        const Data::const_iterator min = std::min_element(qualities.begin(),
            qualities.end());
        const Index index = (Index)(min - qualities.begin());
        splitterIndices.push_back(index);
    }
    return splitterIndices;
}

static Indices ComplementToFullRange(const Indices& indices)
{
    const Index mzOfRightmostSplitterComponent =
        *std::max_element(indices.begin(), indices.end());
    const Index mzOfLeftmostSplitterComponent =
        *std::min_element(indices.begin(), indices.end());

    return range(mzOfLeftmostSplitterComponent,
        mzOfRightmostSplitterComponent + 1u, 1u);
}

// Following function assumes that components are sorted by means.
static MixtureModel ExtractSplitter(MixtureModel& components,
    DataType splittingPeakMz)
{
    const unsigned componentsNum = (unsigned)components.size();
    Data gaussianQualities =
        ComputeComponentQualities(components, splittingPeakMz);

    Indices splitterIndices = FindSplitterIndices(gaussianQualities);
    splitterIndices = ComplementToFullRange(splitterIndices);

    GaussianMixtureModel splitter = filter<GaussianComponent>(
      gsl::span<GaussianComponent>(components), IndicesView(splitterIndices));

    return splitter;
}
}
