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
    Signal FWHHCalculator::GetLeftFWHH(const SignalView x, const SignalView y, const IndicesView valleys, const IndicesView peaks)
    {
        Signal result;
        result.reserve(peaks.size());

        for (int i = 0; i < peaks.size(); ++i)
        {
            Index peakIdx = peaks[i];
            Index valleyIdx = valleys[i];
            DataType peakVal = y[peakIdx];

            SignalView segment = y.subspan(valleyIdx, peakIdx - valleyIdx);
            DataType lfwhhPeakVal = peakVal * 0.5;
            Index lfwhhNearestIdx = GetClosestNeighbourIndex(segment, lfwhhPeakVal) + valleyIdx;
            DataType lfwhhVal = Lerp(y[lfwhhNearestIdx], y[lfwhhNearestIdx + 1], x[lfwhhNearestIdx], x[lfwhhNearestIdx + 1], lfwhhPeakVal);

            result.push_back(lfwhhVal);
        }

        return result;
    }

    Signal FWHHCalculator::GetRightFWHH(const SignalView x, const SignalView y, const IndicesView valleys, const IndicesView peaks)
    {
        Signal result;
        result.reserve(peaks.size());

        for (int i = 0; i < peaks.size(); ++i)
        {
            Index peakIdx = peaks[i];
            Index valleyIdx = valleys[i + 1];
            DataType peakVal = y[peakIdx];

            SignalView segment = y.subspan(peakIdx, valleyIdx - peakIdx);
            DataType rfwhhPeakVal = peakVal * 0.5;
            Signal segmentInversed = spectre::core::functional::transform<DataType>(segment, [](DataType val) { return -val; });
            Index rfwhhNearestIdx = GetClosestNeighbourIndex(segmentInversed, -rfwhhPeakVal) + peakIdx;
            DataType rfwhhVal = Lerp(y[rfwhhNearestIdx], y[rfwhhNearestIdx + 1], x[rfwhhNearestIdx], x[rfwhhNearestIdx + 1], rfwhhPeakVal);

            result.push_back(rfwhhVal);
        }

        return result;
    }

    inline Index FWHHCalculator::GetClosestNeighbourIndex(const SignalView & xSorted, DataType value)
    {
        Index valueIdx = 0;

        for (unsigned i = 0; i < unsigned(xSorted.size()); ++i)
        {
            bool isGreater = xSorted[i] < value;
            unsigned cond = unsigned(!isGreater) - 1;
            i = i + (cond & xSorted.size());
            valueIdx = (i - 1) & cond;
        }

        return valueIdx;
    }

    inline DataType FWHHCalculator::Lerp(DataType x1, DataType x2, DataType y1, DataType y2, DataType x)
    {
        DataType slope = (y2 - y1) / (x2 - x1);
        DataType result = y1 + (x - x1) * slope;
        return result;
    }
}
