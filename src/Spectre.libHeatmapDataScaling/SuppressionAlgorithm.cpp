/*
* SupressionAlgorithm.cpp
* Class with suppresion algorithm implementation for contrast enhancement.
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

#include "Spectre.libHeatmapDataScaling/SuppressionAlgorithm.h"

namespace spectre::visualization
{
SuppressionAlgorithm::SuppressionAlgorithm(const double topPercent) : topPercent(topPercent) { }

std::vector<double> SuppressionAlgorithm::scaleData(const gsl::span<double> intensities)
{
    std::vector<double> newIntensities;
    newIntensities.reserve(intensities.size());
    const double maxIntensity = *max_element(std::begin(intensities), std::end(intensities));
    const double cutoff = calculateQuantile(intensities, 1 - topPercent);
    for (int i = 0; i < intensities.size(); i++)
    {
        if (intensities[i] > cutoff)
            newIntensities.push_back(cutoff);
        else
            newIntensities.push_back(maxIntensity * intensities[i] / cutoff);
    }
    return newIntensities;
}

double SuppressionAlgorithm::calculateQuantile(gsl::span<double> intensities, const double probability)
{
    const int size = static_cast<int>(intensities.size());
    std::vector<double> sortedIntensities(size);
    for (int i = 0; i < size; i++)
    {
        sortedIntensities[i] = intensities[i];
    }
    std::sort(sortedIntensities.begin(), sortedIntensities.end());
    const double index = 1 + (size - 1) * probability;
    const int indexFloor = static_cast<int>(floor(index));
    const int indexCeiling = static_cast<int>(ceil(index));
    double quantileValue = sortedIntensities[indexFloor - 1];
    if ((index > indexFloor) && (sortedIntensities[indexCeiling - 1] != quantileValue))
    {
        const double fractionalPart = index - indexFloor;
        quantileValue = (1 - fractionalPart) * quantileValue + fractionalPart * sortedIntensities[indexCeiling - 1];
    }
    return quantileValue;
}
}