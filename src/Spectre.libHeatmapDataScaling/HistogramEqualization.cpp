/*
* HistogramEqualization.cpp
* Class with histogram equalization algorithm implementation for contrast enhancement.
*
Copyright 2018 Daniel Babiak
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

#include "Spectre.libException/ArgumentOutOfRangeException.h"
#include "Spectre.libFunctional/Transform.h"
#include "Spectre.libHeatmapDataScaling/HistogramEqualization.h"

namespace spectre::visualization
{
std::vector<double> HistogramEqualization::scaleData(const gsl::span<const double> intensities) const
{
    const auto minIntensitiesRange = *min_element(std::begin(intensities), std::end(intensities));
    const auto maxIntensitiesRange = *max_element(std::begin(intensities), std::end(intensities));

    if (minIntensitiesRange < minRangeRestriction)
    {
        throw core::exception::ArgumentOutOfRangeException<double>("minIntensitiesRange", minRangeRestriction, maxRangeRestriction, minIntensitiesRange);
	}
    if (maxIntensitiesRange > maxRangeRestriction)
    {
        throw core::exception::ArgumentOutOfRangeException<double>("maxIntensitiesRange", minRangeRestriction, maxRangeRestriction, maxIntensitiesRange);
    }

    std::vector<uint8_t> roundedIntensities = spectre::core::functional::transform<double, uint8_t>(intensities, [](double intensity) { return static_cast<uint8_t>(intensity + 0.5); });
    std::vector<unsigned int> intensitiesOccurences = countRepeatingValues(roundedIntensities);
    std::vector<unsigned int> cumulativeDistribution(intensitiesOccurences.size());
    std::partial_sum(intensitiesOccurences.begin(), intensitiesOccurences.end(), cumulativeDistribution.begin());
    return calculateNewHistogramData(roundedIntensities, cumulativeDistribution);
}

std::vector<unsigned int> HistogramEqualization::countRepeatingValues(const gsl::span<uint8_t> intensities) const
{
    std::vector<unsigned int> histogram(maxRangeRestriction + 1);
    for (int i = 0; i < intensities.size(); ++i) {
        ++histogram[static_cast<size_t>(intensities[i])];
    }
    return histogram;
}

std::vector<double> HistogramEqualization::calculateNewHistogramData(const gsl::span<const uint8_t> intensities, const std::vector<unsigned int> &cumulativeDistribution) const
{
    const size_t size = intensities.size();
    std::vector<double> newIntensities(size);
    const unsigned int minCumulativeDistributionValue = *min_element(std::begin(cumulativeDistribution), std::end(cumulativeDistribution));
    for (size_t i = 0; i < size; i++)
    {
        const double cumulativeDistributionValue = static_cast<double>(cumulativeDistribution[static_cast<int>(intensities[i])]);
        newIntensities[i] = calculateNewIntensity(cumulativeDistributionValue, minCumulativeDistributionValue, size);
    }
    return newIntensities;
}
}
