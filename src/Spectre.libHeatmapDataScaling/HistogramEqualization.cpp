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

#include "Spectre.libFunctional/Transform.h"
#include "Spectre.libHeatmapDataScaling/HistogramEqualization.h"

namespace spectre::visualization
{
std::vector<double> HistogramEqualization::scaleData(const gsl::span<const double> intensities)
{
    std::vector<double> roundedIntensities = spectre::core::functional::transform(intensities, [](double intensity) { return static_cast<int>(intensity + 0.5); });
    std::vector<uint8_t> intensitiesOccurances = countRepeatingValues(roundedIntensities);
    std::vector<uint8_t> cumulativeDistribution = std::accumulate(intensitiesOccurances.begin(), intensitiesOccurances.end(), cumulativeDistribution, [](std::vector<uint8_t> accumulator, uint8_t intensityOccurances)
    {
            if (accumulator.size() == 0)
                accumulator.push_back(intensityOccurances);
            else accumulator.push_back(accumulator[accumulator.size() - 1] + intensityOccurances);
            return accumulator;
    });
    return calculateNewHistogramData(roundedIntensities, cumulativeDistribution);
}

std::vector<uint8_t> HistogramEqualization::countRepeatingValues(const gsl::span<double> intensities)
{
    std::vector<uint8_t> histogram(256);
    for (int i = 0; i < intensities.size(); ++i) {
        histogram[static_cast<uint8_t>(intensities[i])]++;
    }
    return histogram;
}

std::vector<double> HistogramEqualization::calculateNewHistogramData(const gsl::span<double> intensities, const std::vector<uint8_t> cumulativeDistribution)
{
    size_t const size = intensities.size();
    std::vector<double> newIntensities(size);
    const uint8_t minCumulativeDistributionValue = *min_element(std::begin(cumulativeDistribution), std::end(cumulativeDistribution));
    for (auto i = 0; i < size; i++)
    {
        newIntensities[i] = round(((static_cast<double>(cumulativeDistribution[static_cast<int>(intensities[i])]) - minCumulativeDistributionValue) /
            (size - minCumulativeDistributionValue)) * 255);
    }
    return newIntensities;
}
}
