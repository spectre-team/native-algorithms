/*
* Normalization.cpp
* Class with normalization algorithm implementation.
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

#include "Spectre.libHeatmapDataScaling/Normalization.h"

namespace spectre::visualization
{
Normalization::Normalization(const int minIntensityRange, const int maxIntensityRange) :
    minIntensityRange(minIntensityRange), maxIntensityRange(maxIntensityRange), intensityRange(maxIntensityRange - minIntensityRange) { }

std::vector<double> Normalization::scaleData(const gsl::span<const double> intensities)
{
    std::vector<double> newIntensities(intensities.size());
    double const oldMin = *min_element(std::begin(intensities), std::end(intensities));
    double const oldMax = *max_element(std::begin(intensities), std::end(intensities));
    double const oldRange = oldMax - oldMin;

    for (int i = 0; i< intensities.size(); i++)
    {
        newIntensities[i] = ((intensities[i] - oldMin) * (intensityRange) / (oldRange)) + minIntensityRange;
    }
    return newIntensities;
}
}