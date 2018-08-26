/*
* FWHHCalculator.cpp
* Class for calculating locations of full width at half height markers for
* input peak data.
*
Copyright 2018 Dariusz Kuchta

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

#include "FWHHCalculator.h"
#include "Spectre.libFunctional/Transform.h"

namespace spectre::algorithm::peakfinder
{
static inline bool IsInRange(DataType begin, DataType end, DataType x)
{
    return begin <= x && x <= end;
}

    Data FWHHCalculator::GetLeftFWHH(const DataView x, const DataView y, const IndicesView valleys, const IndicesView peaks)
    {
        Data result(peaks.size());

        for (int i = 0; i < peaks.size(); ++i)
        {
            const Index peakIdx = peaks[i];
            const Index valleyIdx = *(std::lower_bound(valleys.begin(), valleys.end(), peakIdx) - 1);
            const DataType peakVal = y[peakIdx];

            const DataView segment = y.subspan(valleyIdx, peakIdx - valleyIdx);
            const DataType lfwhhPeakVal = peakVal * 0.5;
            Data segmentNegated = spectre::core::functional::transform<DataType>(segment, [](DataType val) { return -val; });
            const Index lfwhhNearestIdx = GetClosestNeighbourIndex(segmentNegated, -lfwhhPeakVal) + valleyIdx;
            const DataType begin = x[lfwhhNearestIdx];
            const DataType end = x[lfwhhNearestIdx + 1];
            const DataType interpolated = LinearInterpolation(y[lfwhhNearestIdx], y[lfwhhNearestIdx + 1], begin, end, lfwhhPeakVal);
            result[i] = IsInRange(begin, end, interpolated) ? interpolated : x[valleyIdx];
        }

        return result;
    }

    Data FWHHCalculator::GetRightFWHH(const DataView x, const DataView y, const IndicesView valleys, const IndicesView peaks)
    {
        Data result(peaks.size());

        for (int i = 0; i < peaks.size(); ++i)
        {
            const Index peakIdx = peaks[i];
            const Index valleyIdx = *std::upper_bound(valleys.begin(), valleys.end(), peakIdx);
            const DataType peakVal = y[peakIdx];

            const DataView segment = y.subspan(peakIdx, valleyIdx - peakIdx);
            const DataType rfwhhPeakVal = peakVal * 0.5;
            const Index rfwhhNearestIdx = GetClosestNeighbourIndex(segment, rfwhhPeakVal) + peakIdx;
            const DataType begin = x[rfwhhNearestIdx];
            const DataType end = x[rfwhhNearestIdx + 1];
            const DataType interpolated = LinearInterpolation(y[rfwhhNearestIdx], y[rfwhhNearestIdx + 1], begin, end, rfwhhPeakVal);
            result[i] = IsInRange(begin, end, interpolated) ? interpolated : x[valleyIdx - 1];
        }

        return result;
    }

    inline Index FWHHCalculator::GetClosestNeighbourIndex(const DataView & xSorted, DataType value)
    {
        Index valueIdx = 0;

        // Branchless breakable loop
        for (unsigned i = 0; i < unsigned(xSorted.size()); ++i)
        {
            // First we check the break condition and save result
            bool isGreater = xSorted[i] < value;
            // We convert the negated result to unsigned and subtract 1 to either have 0x0 or 0xFFFFFFFF
            unsigned cond = unsigned(!isGreater) - 1;
            // If the condition is true then we bit-and the desired index with full-set mask
            valueIdx = (i - 1) & cond;
            // If the condtion is true then the loop variable is increased so the block breaks on next check.
            i = i + (cond & xSorted.size());
        }

        return valueIdx;
    }

    inline DataType FWHHCalculator::LinearInterpolation(DataType x1, DataType x2, DataType y1, DataType y2, DataType x)
    {
        DataType slope = (y2 - y1) / (x2 - x1);
        DataType result = y1 + (x - x1) * slope;
        return result;
    }
}
