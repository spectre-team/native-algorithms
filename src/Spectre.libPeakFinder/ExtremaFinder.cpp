/*
 * ExtremaFinder.cpp
 * Finding indices of extrema in signals.
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
#include <functional>
#include "ExtremaFinder.h"
#include "Spectre.libStatistics/Math.h"
#include "Spectre.libFunctional/Filter.h"

namespace spectre::algorithm::peakfinder
{
using namespace spectre::statistics::basic_math;
using namespace spectre::core::functional;

// Branchless cache-efficient zero filter
static inline Indices GetNonZeros(const Data& data)
{
    const size_t length = data.size();
    std::vector<unsigned> indices(length + 1);
    unsigned zeros = 0;

    for (unsigned i = 0; i < length; i++)
    {
        bool dataIsZero = data[i] == 0.0;
        int index = i - zeros + dataIsZero;
        zeros += dataIsZero;

        indices[index] = i;
    }
    indices.resize(length - zeros);

    return indices;
}

// This function finds indices of intensities that combined together create signal with removed flat lines.
static inline Indices FilterFlatSegmentIndices(const DataView& signal)
{
    Data derivatives = differentiate(signal);
    Indices indicesOfNonZeros = GetNonZeros(derivatives);
    indicesOfNonZeros.push_back((unsigned)(signal.size() - 1));
    return indicesOfNonZeros;
}

static inline Data DifferentiateFilteredSignal(const DataView& signal, const Indices& indices)
{
    Data filteredSignal = filter<DataType, Index>(signal, gsl::make_span(indices));
    return differentiate_unsafe(gsl::make_span(filteredSignal)); // Unsafe is used here, because the exception
}                                                              // would've already been thrown in FilterFlatSegmentIndices

// Comparison operations are interchangably std::greater_equal and std::less_equal
// skipFirstAndLast is added for the purpose of peak finding, when the peak is
// assumed to never start or end of the signal.
using ComparisonOp = std::function<bool(DataType, DataType)>;
static Indices FindExtrema(const DataView& signal, ComparisonOp leftComparison,
    ComparisonOp rightComparison, bool skipFirstAndLast = false)
{
    Indices filteredIndices = FilterFlatSegmentIndices(signal);
    Data derivatives = DifferentiateFilteredSignal(signal, filteredIndices);

    Indices extrema;
    // special case for 0th element
    if (!skipFirstAndLast && rightComparison(derivatives[0], 0.0))
        extrema.push_back(filteredIndices[0]);

    // regular processsing for finding extrema
    for (unsigned i = 1; i < derivatives.size(); i++)
    {
        if (leftComparison(derivatives[i - 1], 0.0) && rightComparison(derivatives[i], 0.0))
            extrema.push_back(filteredIndices[i]);
    }

    // special case for nth element
    if (!skipFirstAndLast && leftComparison(*(derivatives.end() - 1), 0.0))
        extrema.push_back(filteredIndices[derivatives.size()]);

    return extrema;
}

Indices ExtremaFinder::FindValleys(const DataView & signal) const
{
    return FindExtrema(signal, std::less_equal<DataType>(), std::greater_equal<DataType>());
}
Indices ExtremaFinder::FindPeaks(const DataView & signal) const
{
    return FindExtrema(signal, std::greater_equal<DataType>(), std::less_equal<DataType>(), true);
}
}