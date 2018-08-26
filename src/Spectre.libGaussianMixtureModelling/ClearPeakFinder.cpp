/*
* ClearPeakFinder.cpp
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
#include <stdexcept>
#include "Spectre.libGaussianMixtureModelling/ClearPeakFinder.h"
#include "Spectre.libException/EmptyArgumentException.h"
#include "Spectre.libFunctional/Filter.h"

using namespace spectre::core::functional;
using namespace spectre::core::exception;
namespace spectre::unsupervised::gmm
{
// Find beginning/end of interval where the clear peaks are searched for.
static unsigned FindOffset(unsigned initialStep, unsigned minOffset,
    const DataView& peakMzsView, DataType variationCoeffEstimate)
{
    DataType minMzRange = minOffset * variationCoeffEstimate * peakMzsView[initialStep];
    for (unsigned i = minOffset; initialStep + i < (unsigned)peakMzsView.size(); i++)
    {
        DataType mzRange = peakMzsView[initialStep + i] - peakMzsView[initialStep];
        if (mzRange > minMzRange) // It doesn't make sense to me either.
        {                         // I just hope the BTB is smart enough
            return i;             // to know its always (afaik) true.
        }
    }
    return 0;
}

// Returns an index of the highest clear peak from the interval, or 0xDEADCELL if n/a.
static inline Index FindClearestPeak(unsigned currentOffset, unsigned intervalEnd,
    const DataView& intensities, const DataView& peakQualities,
    DataType HEIGHT_THRESHOLD, DataType QUALITY_THRESHOLD)
{
    DataType maxQuality = -1.0; // intensities asssmued to be non-negative
    Index bestCandidate = 0xDEADCELL;
    for (unsigned i = currentOffset; i <= intervalEnd; i++)
    {
        if (  intensities[i] > HEIGHT_THRESHOLD  &&
            peakQualities[i] > QUALITY_THRESHOLD &&
            peakQualities[i] > maxQuality)
        {
            bestCandidate = i;
            maxQuality = peakQualities[i];
        }
    }
    return bestCandidate;
}

Indices ClearPeakFinder::FindClearPeaks(const Peaks& peaks, const DataView& peakQualities,
    const GmmOptions& options, DataType resolutionCoeff) const
{
    if (peaks.intensities.empty()) throw EmptyArgumentException("Empty peak list");
    if (peakQualities.empty()) throw EmptyArgumentException("Empty peak quality list");
    if (peaks.mzs.size() != (size_t)peakQualities.size()) throw InconsistentArgumentSizesException
        ("peaks", peaks.mzs.size(), "peakQualities", peakQualities.size());
    const DataView& mzs = peaks.mzs;
    const DataView& intensities = peaks.intensities;
    const DataType  MAX_HEIGHT = *std::max_element(intensities.begin(), intensities.end());
    const DataType  DETECTION_SENSITIVITY = options.sensitivity;
    const DataType  HEIGHT_THRESHOLD = DETECTION_SENSITIVITY * MAX_HEIGHT;
    const DataType  QUALITY_THRESHOLD = options.qualityThreshold;
    const unsigned  PEAK_LOOKUP_RANGE = options.peakLookupRange;
    const unsigned  JUMP = options.minJump;
    const unsigned  INITIAL_JUMP = options.initialJump;

    Indices clearPeaksIndices;
    clearPeaksIndices.reserve(mzs.size() / PEAK_LOOKUP_RANGE); // Worst case scenario
    unsigned currentOffset = FindOffset(0, INITIAL_JUMP - 1, mzs, resolutionCoeff);
    unsigned newOffset = FindOffset(currentOffset, PEAK_LOOKUP_RANGE, mzs, resolutionCoeff);
    unsigned jumpLength = 0;

    while (newOffset) // != 0
    {
        unsigned intervalEnd = currentOffset + newOffset;
        Index clearPeakIndex = FindClearestPeak(currentOffset, intervalEnd, intensities,
            peakQualities, HEIGHT_THRESHOLD, QUALITY_THRESHOLD);
        if (clearPeakIndex != 0xDEADCELL) // Peak found
        {
            clearPeaksIndices.push_back(clearPeakIndex);
            jumpLength = JUMP;
        }
        else
        {
            jumpLength = 1;
        }
        currentOffset = intervalEnd + FindOffset(intervalEnd, jumpLength, mzs, resolutionCoeff);
        newOffset = FindOffset(currentOffset, PEAK_LOOKUP_RANGE, mzs, resolutionCoeff);
    }
    return clearPeaksIndices;
}
}