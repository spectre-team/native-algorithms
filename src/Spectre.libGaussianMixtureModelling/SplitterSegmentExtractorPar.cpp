/*
 * SplitterSegmentExtractor.cpp
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
#include "Spectre.libGaussianMixtureModelling/SplitterSegmentExtractorPar.h"
#include "Spectre.libGaussianMixtureModelling/GaussianDistribution.h"

namespace spectre::unsupervised::gmm
{
static inline void ExtendIntervalToRight(Index& intervalEnd, DataType minMzRange,
    SpectrumView spectrum)
{
    Index extendedRangeIndex = intervalEnd;
    DataType minIntensity = spectrum.intensities[intervalEnd];
    while (extendedRangeIndex < (unsigned)spectrum.mzs.size() - 1 &&
           spectrum.mzs[++extendedRangeIndex] <= minMzRange)
    {
        if (minIntensity > spectrum.intensities[extendedRangeIndex])
        {
            minIntensity = spectrum.intensities[extendedRangeIndex];
            intervalEnd = extendedRangeIndex;
        }
    }
}

// It is assumed that spectrum.mzs.size() > intervalStart.
static inline void ExtendIntervalToLeft(Index& intervalStart, DataType minMzRange,
    SpectrumView spectrum)
{
    Index extendedRangeIndex = 0; // Find index of first point extending the range
    while (extendedRangeIndex < (unsigned)spectrum.mzs.size() &&
        spectrum.mzs[extendedRangeIndex++] <= minMzRange) {};
    extendedRangeIndex--; // Reset index to first satisfactory mz index

    const DataType startingMz = spectrum.mzs[intervalStart];
    DataType minIntensity = spectrum.intensities[extendedRangeIndex];
    while (extendedRangeIndex < (unsigned)spectrum.mzs.size() - 1 &&
           spectrum.mzs[++extendedRangeIndex] <= startingMz)
    {
        if (minIntensity > spectrum.intensities[extendedRangeIndex])
        {
            minIntensity = spectrum.intensities[extendedRangeIndex];
            intervalStart = extendedRangeIndex;
        }
    }
}

constexpr int CLEAR_PEAK_SEARCH_RANGE = 5;

static inline Index GetIntervalEnd(SpectrumView spectrum, IndicesView valleyIndices,
    Index indexOfSplittingPeak, DataType resolutionCoeff, size_t peaksNum)
{
    Index rightmostValleyIndex = std::min((unsigned)peaksNum - 1,
        indexOfSplittingPeak + CLEAR_PEAK_SEARCH_RANGE - 1);
    Index intervalEnd = valleyIndices(rightmostValleyIndex) - 1;

    DataType lastMz = spectrum.mzs[intervalEnd];
    DataType splittingPeakMz = spectrum.mzs[indexOfSplittingPeak];

    DataType minMzRange = CLEAR_PEAK_SEARCH_RANGE*splittingPeakMz*resolutionCoeff;

    if (lastMz - splittingPeakMz < minMzRange)
        ExtendIntervalToRight(intervalEnd, minMzRange, spectrum);

    return intervalEnd;
}

static inline Index GetIntervalStart(SpectrumView spectrum, IndicesView valleyIndices,
    Index indexOfSplittingPeak, DataType resolutionCoeff)
{
    Index leftmostValleyIndex =
        std::max(0, ((int)indexOfSplittingPeak - CLEAR_PEAK_SEARCH_RANGE));
    Index intervalStart = std::max((int)valleyIndices[leftmostValleyIndex] - 1, 0);

    DataType startingMz = spectrum.mzs[intervalStart];
    DataType splittingPeakMz = spectrum.mzs[indexOfSplittingPeak];

    DataType minMzRange = CLEAR_PEAK_SEARCH_RANGE*splittingPeakMz*resolutionCoeff;
    if (splittingPeakMz - startingMz < minMzRange)
        ExtendIntervalToLeft(intervalStart, minMzRange, spectrum);

    return intervalStart;
}

static inline Data GetMzsOfLeftMargin(SpectrumView& spectrum, Index intervalStart,
    DataType resolutionCoeff)
{
    DataView& mzs = spectrum.mzs;
    const DataType step = mzs[intervalStart + 1] - mzs[intervalStart];
    DataType leftMarginMzBegin = mzs[intervalStart] * (1.0 - 6.0*resolutionCoeff);
    DataType leftMarginMzEnd = mzs[intervalStart] - step;
    size_t intervalSize =
        (size_t)ceil((leftMarginMzEnd - leftMarginMzBegin) / step);

    Data margin(intervalSize);
    DataType floatIter = leftMarginMzBegin;
    for (Index i = 0; i < intervalSize; i++, floatIter += step)
    {
        margin[i] = floatIter;
    }

    return margin;
}

constexpr DataType TWO_SQRTS_OF_2_PI = 5.0132565492620005; // precomp., since sqrt is
                                                           // not a constepxr function
static inline Data GetMzsOfRightMargin(SpectrumView& spectrum, Index intervalEnd,
    DataType resolutionCoeff)
{
    DataView& mzs = spectrum.mzs;
    const DataType step = mzs[intervalEnd] - mzs[intervalEnd - 1];
    DataType marginMzBegin = mzs[intervalEnd] + step;
    DataType marginMzEnd = mzs[intervalEnd] + 6 * resolutionCoeff*mzs[intervalEnd];
    size_t intervalSize =
        (size_t)ceil((marginMzEnd - marginMzBegin) / step);

    Data margin(intervalSize);
    DataType floatIter = marginMzBegin;
    for (Index i = 0; i < intervalSize; i++, floatIter += step)
    {
        margin[i] = floatIter;
    }

    return margin;
}

static inline Data GetIntensitiesOfLeftMargin(SpectrumView spectrum, DataView margin,
    Index intervalBegin, Index intervalEnd, DataType resolutionCoeff)
{
    const DataView& mzs = spectrum.mzs;

    const DataType heightScaleFactor = TWO_SQRTS_OF_2_PI * resolutionCoeff *
        (mzs[intervalEnd] * spectrum.intensities[intervalEnd]);
    const DataType variance = 2 * resolutionCoeff * mzs[intervalBegin];
    const DataType mean = 0;

    Data marginIntensities(margin.size());
#pragma omp parallel for
    for (int i = 0; i < (int)margin.size(); i++)
    {
        marginIntensities[i] = heightScaleFactor *
            Gaussian(margin[i] - mzs[intervalBegin], mean, variance);
    }

    return marginIntensities;
}

static inline Data GetIntensitiesOfRightMargin(SpectrumView spectrum, DataView margin,
    Index intervalBegin, Index intervalEnd, DataType resolutionCoeff)
{
    const DataView& mzs = spectrum.mzs;

    const DataType heightScaleFactor = TWO_SQRTS_OF_2_PI * resolutionCoeff *
        (mzs[intervalBegin] * spectrum.intensities[intervalBegin]);
    const DataType variance = 2 * resolutionCoeff * mzs[intervalEnd];
    const DataType mean = 0;

    Data marginIntensities(margin.size());
#pragma omp parallel for
    for (int i = 0; i < (int)margin.size(); i++)
    {
        marginIntensities[i] = heightScaleFactor *
            Gaussian(margin[i] - mzs[intervalEnd], mean, variance);
    }

    return marginIntensities;
}

static inline Spectrum CombineSplitterSegment(Data& leftMzMargin,
    Data& leftIntensitiesMargin, SpectrumView& spectrum, Data& rightMzMargin,
    Data& rightIntensitiesMargin, Index intervalStart, Index intervalEnd)
{
    Spectrum result;
    result.mzs = std::move(leftMzMargin);
    result.mzs.insert(result.mzs.end(), spectrum.mzs.begin() + intervalStart,
        spectrum.mzs.begin() + intervalEnd + 1);
    result.mzs.insert(result.mzs.end(), rightMzMargin.begin(), rightMzMargin.end());

    result.intensities = std::move(leftIntensitiesMargin);
    result.intensities.insert(result.intensities.end(),
        spectrum.intensities.begin() + intervalStart,
        spectrum.intensities.begin() + intervalEnd + 1);
    result.intensities.insert(result.intensities.end(), rightIntensitiesMargin.begin(),
        rightIntensitiesMargin.end());

    return result;
}

Spectrum SplitterSegmentExtractorPar::ExtractSplitterSegment(const Peaks & peaks,
    SpectrumView spectrum, IndicesView valleyIndices, Index indexOfSplittingPeak,
    DataType resolutionCoeff) const
{
    Index intervalStart = GetIntervalStart(spectrum, valleyIndices,
        indexOfSplittingPeak, resolutionCoeff);
    Index intervalEnd = GetIntervalEnd(spectrum, valleyIndices,
        indexOfSplittingPeak, resolutionCoeff, peaks.mzs.size());

    Data leftMzMargin = GetMzsOfLeftMargin(spectrum, intervalStart, resolutionCoeff);
    Data rightMzMargin = GetMzsOfRightMargin(spectrum, intervalEnd, resolutionCoeff);
    Data leftIntensitiesMargin = GetIntensitiesOfLeftMargin(spectrum, leftMzMargin,
        intervalStart, intervalEnd, resolutionCoeff);
    Data rightIntensitiesMargin = GetIntensitiesOfRightMargin(spectrum, rightMzMargin,
        intervalStart, intervalEnd, resolutionCoeff);

    Spectrum splitSegment = CombineSplitterSegment(leftMzMargin, leftIntensitiesMargin,
        spectrum, rightMzMargin, rightIntensitiesMargin, intervalStart,
        intervalEnd);
    return splitSegment;
}
}