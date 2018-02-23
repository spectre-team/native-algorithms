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
    
    std::vector<uint8_t> cumulativeDistribution = std::accumulate(intensitiesOccurances.begin(), intensitiesOccurances.end(), cumulativeDistribution, 
        [](std::vector<uint8_t> accumulator, uint8_t next) {
            if (accumulator.size() == 0)
                accumulator.push_back(next);
            else accumulator.push_back(accumulator[accumulator.size() - 1] + next);
            return accumulator;
        });
    //return calculateNewHistogramData(roundedIntensities, histogramMap);
    std::vector<double> returntest;
    return returntest;
}

std::vector<uint8_t> HistogramEqualization::countRepeatingValues(const gsl::span<double> intensities)
{
    std::vector<uint8_t> histogram(256);
    for (int i = 0; i < intensities.size(); ++i) {
        histogram[static_cast<uint8_t>(intensities[i])]++;
    }
    return histogram;
}

std::vector<double> HistogramEqualization::calculateNewHistogramData(const gsl::span<double> intensities, std::map<double, unsigned int> histogramMap)
{
    int const size = static_cast<int>(intensities.size());
    std::vector<double> newIntensities(size);
    std::pair<double, unsigned int> const minCumulativeDistributionValue = (*std::min_element(histogramMap.begin(), histogramMap.end(),
        [](decltype(histogramMap)::value_type& l, decltype(histogramMap)::value_type& r) -> bool { return l.second < r.second; }));
    for (auto i = 0; i < size; i++)
    {
        newIntensities[i] = (round((histogramMap.find(intensities[i])->second - static_cast<double>(minCumulativeDistributionValue.second)) /
            (size - static_cast<double>(minCumulativeDistributionValue.second)) * 255));
    }
    return newIntensities;
}
}
