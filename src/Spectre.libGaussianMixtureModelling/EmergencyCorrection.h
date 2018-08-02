/*
 * EmergencyCorrection.h
 * Performs correction in case of overlapping splitters.
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
#include "Spectre.libStatistics/Math.h"
#include "Spectre.libStatistics/Statistics.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationMaximization.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/MaximizationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/ModelUtils.h"

namespace spectre::unsupervised::gmm
{

static Index GetLeftBoundaryAndSplitter(Index, Splitter&, SplittersView,
    SpectrumView);
static Index GetRightBoundaryAndSplitter(Index, Splitter&, SplittersView,
    SpectrumView);
static void UpdateLeftMargin(Spectrum&, const Splitter&,IndicesView,
    IndicesView, Index, Index);
static void UpdateRightMargin(Spectrum&, const Splitter&, IndicesView,
    IndicesView, Index, Index);
static GaussianMixtureModel JoinSplitters(const Splitter&, const Splitter&);
static void RemodelSegment(const Spectrum&, GaussianMixtureModel&, DataType,
    const GmmOptions&);
static void UpdateLeftSplitter(Index, Splitters&, const GaussianMixtureModel&,
    size_t);
static void UpdateRightSplitter(Index, Splitters&, const GaussianMixtureModel&,
    size_t);

/// <summary>
/// In the case when two splitters overlap, the function recalculates them
/// so that they no longer do and saves them to the splitters array.
/// </summary>
/// <param name="segmentIndex">Index of segment for which overlap occured at.
/// </param>
/// <param name="spectrum">Spectrum to operate on.</param>
/// <param name="splitters">Array of all splitters</param>
/// <param name="clearPeaksIndices">Indices of splitting peaks (rel. to
/// peaks indices).</param>
/// <param name="peakIndices">Indices of peaks in the signal (rel. to
/// the signal).</param>
/// <param name="resolutionCoefficient">Resolution coefficient.</param>
/// <param name="options">Predefined GMM Options.</param>
inline void EmergencyCorrection(Index segmentIndex, SpectrumView spectrum,
    Splitters& splitters, IndicesView clearPeaksIndices,
    IndicesView peakIndices, DataType resolutionCoefficient,
    const GmmOptions& options)
{
    Splitter leftSplitter, rightSplitter;

    const Index leftBoundary = GetLeftBoundaryAndSplitter(segmentIndex,
        leftSplitter, splitters, spectrum);
    const Index rightBoundary = GetRightBoundaryAndSplitter(segmentIndex,
        rightSplitter, splitters, spectrum);

    const unsigned length = rightBoundary - leftBoundary;
    Spectrum segment = spectrum.subspan(leftBoundary, length).ToSpectrum();

    UpdateLeftMargin(segment, leftSplitter, peakIndices, clearPeaksIndices,
        leftBoundary, segmentIndex);
    UpdateRightMargin(segment, rightSplitter, peakIndices, clearPeaksIndices,
        rightBoundary, segmentIndex);

    GaussianMixtureModel joinedSplitters = JoinSplitters(leftSplitter,
        rightSplitter);

    RemodelSegment(segment, joinedSplitters, resolutionCoefficient, options);

    UpdateLeftSplitter(segmentIndex, splitters, joinedSplitters,
        leftSplitter.size());
    UpdateRightSplitter(segmentIndex, splitters, joinedSplitters,
        leftSplitter.size());
}

static DataType FindBoundary(const Splitter& splitter, bool isLeft)
{
    std::function<DataType(DataType, DataType)> arithOp;
    std::function<Data::iterator(Data::iterator, Data::iterator)> extremumOp;

    if(isLeft)
    {
        arithOp = std::minus<>();
        extremumOp = std::min_element<Data::iterator>;
    }
    else
    {
        arithOp = std::plus<>();
        extremumOp = std::max_element<Data::iterator>;
    }

    constexpr DataType reasonableRange = 3.0;
    Data shifted(splitter.size());
    for(Index i = 0; i < shifted.size(); i++)
    {
        shifted[i] =
            arithOp(splitter[i].mean, reasonableRange * splitter[i].deviation);
    }
    const DataType boundary = *extremumOp(shifted.begin(), shifted.end());
    return boundary;
}

static Index GetLeftBoundaryAndSplitter(Index segmentIndex,
    Splitter& leftSplitter, SplittersView splitters, SpectrumView spectrum)
{
    const Index leftSplitterIndex = segmentIndex - 1;
    Index leftBoundary;
    if (segmentIndex > 0)
    {
        leftSplitter = splitters[leftSplitterIndex];
        if (leftSplitter.empty())
        {
            leftBoundary = 0;
        }
        else
        {
            const DataType mzBoundary = FindBoundary(leftSplitter, true);
            leftBoundary = (Index)(std::lower_bound(spectrum.mzs.begin(),
                spectrum.mzs.end(), mzBoundary)
                - spectrum.mzs.begin());
        }
    }
    else
    {
        leftBoundary = 0;
    }
    return leftBoundary;
}

static Index GetRightBoundaryAndSplitter(Index segmentIndex,
    Splitter& rightSplitter, SplittersView splitters, SpectrumView spectrum)
{
    const unsigned splitterNum = (unsigned)splitters.size();
    const Index rightSplitterIndex = segmentIndex;
    Index rightBoundary;
    if (segmentIndex < splitterNum)
    {
        rightSplitter = splitters[rightSplitterIndex];
        const DataType mzBoundary = FindBoundary(rightSplitter, false);
        rightBoundary = (Index)(std::upper_bound(spectrum.mzs.begin(),
            spectrum.mzs.end(), mzBoundary)
            - spectrum.mzs.begin());
    }
    else
    {
        rightBoundary = (Index)spectrum.mzs.size();
    }
    return rightBoundary;
}

static void UpdateMargin(Spectrum& segment, const Splitter& splitter,
    IndicesView peakIndices, IndicesView clearPeaksIndices, Index boundary,
    Index segmentIndex, bool isLeft)
{
    const Index splitterIndex = segmentIndex - isLeft;
    Data splitterIntensities =
        ComputeIntensitiesFor(segment.mzs, splitter);
    const Index peakIndex =
        peakIndices[clearPeaksIndices[splitterIndex]];

    Data::iterator srcBeg;
    Data::iterator srcEnd;
    Data::iterator dstBeg;

    if(isLeft)
    {
        srcBeg = splitterIntensities.begin() + boundary;
        srcEnd = splitterIntensities.begin() + 1 + peakIndex;
        dstBeg = segment.intensities.begin() + boundary;
    }
    else
    {
        srcBeg = splitterIntensities.begin() + peakIndex;
        srcEnd = splitterIntensities.begin() + boundary;
        dstBeg = segment.intensities.begin() + peakIndex;
    }

    std::copy(srcBeg, srcEnd, dstBeg);
}

static void UpdateLeftMargin(Spectrum& segment, const Splitter& splitter,
    IndicesView peakIndices, IndicesView clearPeaksIndices, Index boundary,
    Index segmentIndex)
{
    if (segmentIndex > 0)
    {
        UpdateMargin(segment, splitter, peakIndices, clearPeaksIndices,
            boundary, segmentIndex, true);
    }
}

static void UpdateRightMargin(Spectrum& segment, const Splitter& splitter,
    IndicesView peakIndices, IndicesView clearPeaksIndices, Index boundary,
    Index segmentIndex)
{
    const unsigned splitterNum = (unsigned)clearPeaksIndices.size();
    if (segmentIndex < splitterNum)
    {
        UpdateMargin(segment, splitter, peakIndices, clearPeaksIndices,
            boundary, segmentIndex, false);
    }
}

static GaussianMixtureModel JoinSplitters(const Splitter& left,
    const Splitter& right)
{
    GaussianMixtureModel joinedSplitters(left);
    joinedSplitters.insert(joinedSplitters.end(), right.begin(), right.end());
    DataType sum = 0.0;
    DataType correction = 0.0;
    using statistics::basic_math::accurate_add;
    std::for_each(joinedSplitters.begin(), joinedSplitters.end(),
        [&sum, &correction](GaussianComponent& c)
    { sum = accurate_add(sum, c.weight, correction); });
    std::for_each(joinedSplitters.begin(), joinedSplitters.end(),
        [sum](GaussianComponent& c) { c.weight /= sum; });
    return joinedSplitters;
}

static void RemodelSegment(const Spectrum& segment,
    GaussianMixtureModel& splitters, DataType resolutionCoefficient,
    const GmmOptions& options)
{
    using statistics::simple_statistics::Mean;
    const DataType minStd = options.minStd * resolutionCoefficient *
        Mean(DataView(segment.mzs));
    const DataType epsilon = options.emEpsilon;

    ExpectationMaximization<ExpectationRunnerOpt, MaximizationRunnerOpt>
        expectationMaximization(segment, (unsigned)splitters.size());
    splitters = expectationMaximization.EstimateGmm(
        segment, splitters, minStd, epsilon);
    Data modelIntensities = ComputeIntensitiesFor(segment.mzs, splitters);
    const DataType scale =
        ComputeModelScale(segment.intensities, modelIntensities);
    ScaleComponents(splitters, scale);
}

static void UpdateLeftSplitter(Index segmentIndex, Splitters& splitters,
    const GaussianMixtureModel& joinedSplitters, size_t numComponents)
{
    const Index leftSplitterIndex = segmentIndex - 1;
    if (segmentIndex > 0)
    {
        splitters[leftSplitterIndex] = GaussianMixtureModel(
            joinedSplitters.begin(),
            joinedSplitters.begin() + numComponents);
    }
}

static void UpdateRightSplitter(Index segmentIndex, Splitters& splitters,
    const GaussianMixtureModel& joinedSplitters, size_t numComponents)
{
    const Index rightSplitterIndex = segmentIndex;
    const unsigned splitterNum = (unsigned)splitters.size();
    if (segmentIndex < splitterNum)
    {
        splitters[rightSplitterIndex] = GaussianMixtureModel(
            joinedSplitters.begin() + numComponents,
            joinedSplitters.end());
    }
}


}
