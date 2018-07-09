/*
* SegmentExtractor.h
* Extracts the segment from the signal based on the provided
* segment index and sourranding splitters.
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
#include "GaussianMixtureModel.h"
#include "GaussianDistribution.h"

namespace spectre::unsupervised::gmm
{
static Splitter GetLeftSplitter(Index, SplittersView);
static Splitter GetRightSplitter(Index, SplittersView, unsigned);
static Index GetLeftMz(Index, IndicesView, IndicesView);
static Index GetRightMz(Index, IndicesView, SpectrumView, IndicesView);
static Spectrum SliceNewSpectrum(SpectrumView, Index, Index);
static Data ComputeIntensitiesFromModel(DataView, const GaussianMixtureModel&);
static void SubtractSplittersFromSegment(Spectrum&, const Data&, const Data&);
static Index GetNewMzBoundaryIndex(Index, SpectrumView, const Data&, unsigned,
    bool);
static Spectrum RemoveZeros(Spectrum&);

/// <summary>Extracts a segment from between two splitters.</summary>
/// <param name="spectrum">Mzs and intensities of the signal</param>
/// <param name="peakIndices">List of peak indices in the signal.</param>
/// <param name="clearPeakIndices">Indices of the clear peaks w.r.t signal.
/// </param>
/// <param name="segmentIndex">Index of the segment.</param>
/// <param name="splitters">Splitters found in the signal.</param>
/// <returns>An extracted segment.</returns>
inline Spectrum ExtractSegment(SpectrumView spectrum, IndicesView peakIndices,
    IndicesView clearPeakIndices, Index segmentIndex, SplittersView splitters)
{
    const unsigned splitterNum = (unsigned)clearPeakIndices.size();
    const Splitter leftSplitter = GetLeftSplitter(segmentIndex, splitters);
    const Splitter rightSplitter = GetRightSplitter(segmentIndex, splitters,
        splitterNum);

    Index leftMzIndex =
        GetLeftMz(segmentIndex, peakIndices, clearPeakIndices);
    Index rightMzIndex =
        GetRightMz(segmentIndex, peakIndices, spectrum, clearPeakIndices);

    Spectrum segment = SliceNewSpectrum(spectrum, leftMzIndex, rightMzIndex);

    const Data leftSplitterIntensities =
        ComputeIntensitiesFromModel(segment.mzs, leftSplitter);
    const Data rightSplitterIntensities =
        ComputeIntensitiesFromModel(segment.mzs, rightSplitter);

    SubtractSplittersFromSegment(segment, leftSplitterIntensities,
        rightSplitterIntensities);

    leftMzIndex = GetNewMzBoundaryIndex(segmentIndex, segment,
        leftSplitterIntensities, splitterNum, true);
    rightMzIndex = GetNewMzBoundaryIndex(segmentIndex, segment,
        rightSplitterIntensities, splitterNum, false);

    segment = SliceNewSpectrum(segment, leftMzIndex, rightMzIndex);
    segment = RemoveZeros(segment);
    return segment;
}

// We probably don't need this step, but I've left it here
// just in case. 
static void RemoveInsignificantComponents(Splitter& splitter)
{
    const unsigned componentsNum = (unsigned)splitter.size();
    Index lastNonZeroHeight = componentsNum - 1;
    for(Index i = 0; i < componentsNum; i++)
    {
        if(splitter[i].weight > 0.0)
        {
            lastNonZeroHeight = i;
        }
    }
    const unsigned newSize = lastNonZeroHeight + 1;
    splitter.resize(newSize);
}

static Splitter GetLeftSplitter(Index segmentIndex, SplittersView splitters)
{
    Splitter splitter;
    const Index leftSplitterIndex = segmentIndex - 1;
    // First segment
    if (segmentIndex > 0)
    {
        splitter = splitters[leftSplitterIndex];
        RemoveInsignificantComponents(splitter);
    }
    return splitter;
}

static Splitter GetRightSplitter(Index segmentIndex, SplittersView splitters,
    unsigned splitterNum)
{
    Splitter splitter;
    const Index rightSplitterIndex = segmentIndex;
    if (segmentIndex < splitterNum)
    {
        splitter = splitters[rightSplitterIndex];
        RemoveInsignificantComponents(splitter);
    }
    return splitter;
}

static Index GetLeftMz(Index segmentIndex, IndicesView peakIndices,
    IndicesView clearPeakIndices)
{
    Index index;
    const Index leftSplitterIndex = segmentIndex - 1;
    if (segmentIndex > 0)
    {
        index = peakIndices[clearPeakIndices[leftSplitterIndex]];
    }
    else
    {
        index = 0;
    }

    return index;
}

static Index GetRightMz(Index segmentIndex, IndicesView peakIndices,
    SpectrumView spectrum, IndicesView clearPeakIndices)
{
    Index index;
    const Index rightSplitterIndex = segmentIndex;
    if (segmentIndex < (unsigned)clearPeakIndices.size())
    {
        index = peakIndices[clearPeakIndices[rightSplitterIndex]];
    }
    else
    {
        index = (unsigned)spectrum.mzs.size() - 1;
    }

    return index;
}

static Data ComputeIntensitiesFromModel(DataView mzs,
    const GaussianMixtureModel& components)
{
    Data mixture(mzs.size());
    for (Index i = 0; i < components.size(); i++)
    {
        for (Index j = 0; j < mixture.size(); j++)
        {
            mixture[j] += components[i].weight *
                Gaussian(mzs[j], components[i].mean, components[i].deviation);
        }
    }
    return mixture;
}

static void SubtractSplittersFromSegment(Spectrum& segment,
    const Data& leftIntensities, const Data& rightIntensities)
{
    for(Index i = 0; i < segment.mzs.size(); i++)
    {
        segment.intensities[i] -= leftIntensities[i];
        segment.intensities[i] -= rightIntensities[i];
    }
}

static bool TryGettingNewMzBoundaryOfMiddleSeg(const Data& intensities,
    SpectrumView segment, Index& result)
{
    constexpr DataType thresholdCoefficient = 0.05;
    const Index maxIndex = (Index)(
        std::max_element(intensities.begin(), intensities.end())
        - intensities.begin());
    const DataType max = intensities[maxIndex];
    const DataType threshold = max * thresholdCoefficient;
    DataType min = segment.intensities[maxIndex];
    Index    minIndex = maxIndex;
    for (Index i = 0; i < intensities.size(); i++)
    {
        const bool satisfiesThreshold = intensities[i] > threshold;
        const bool isNewMin = segment.intensities[i] < min;
        if (satisfiesThreshold && isNewMin)
        {
            min = segment.intensities[i];
            minIndex = i;
        }
    }
    result = minIndex;
    const bool wasFound = minIndex != maxIndex;
    return wasFound;
}

static Index GetNewMzBoundaryIndex(Index segmentIndex, SpectrumView segment,
    const Data& intensities, unsigned splitterNum, bool isLeftSplitter)
{
    const bool isFirstSegment = segmentIndex == 0;
    const bool isLastSegment = segmentIndex >= splitterNum;
    const bool isMiddleSegment = !(isFirstSegment || isLastSegment);
    const bool isRightSplitterOfFirstSeg = !isLeftSplitter && isFirstSegment;
    const bool isLeftSplitterOfLastSeg = isLeftSplitter && isLastSegment;
    if(isMiddleSegment || isRightSplitterOfFirstSeg || isLeftSplitterOfLastSeg)
    {
        Index newBoundary;
        const bool found = TryGettingNewMzBoundaryOfMiddleSeg(intensities,
                               segment, newBoundary);
        if(found)
        {
            return newBoundary;
        }
    }
    const Index leftBoundary = 0;
    const Index rightBoundary = (Index)intensities.size() - 1;
    if (isLeftSplitter)
    {
        return leftBoundary;
    }
    else
    {
        return rightBoundary;
    }
}

static Spectrum SliceNewSpectrum(SpectrumView current, Index left, Index right)
{
    const unsigned segmentLength = right - left + 1;
    return current.subspan(left, segmentLength).ToSpectrum();
}

static Spectrum RemoveZeros(Spectrum& segment)
{
    const unsigned size = (unsigned)segment.mzs.size();
    Spectrum newSegment;
    newSegment.mzs.reserve(size);
    newSegment.intensities.reserve(size);
    for(unsigned i = 0; i < size; i++)
    {
        if(segment.intensities[i] > 0)
        {
            newSegment.mzs.push_back(segment.mzs[i]);
            newSegment.intensities.push_back(segment.intensities[i]);
        }
    }
    return newSegment;
}
}
