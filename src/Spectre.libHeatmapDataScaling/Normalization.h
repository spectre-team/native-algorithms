/*
* Normalization.h
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

#pragma once
#include <vector>
#include "Spectre.libHeatmapDataScaling/HeatmapDataScalingAlgorithm.h"

namespace spectre::visualization
{
/// <summary>
/// Class containing normalization algorithm implementation.
/// </summary>
class Normalization : public HeatmapDataScalingAlgorithm
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Normalization"/> class.
    /// </summary>
    /// <param name="minIntensityRange">Minimum value of the range.</param>
    /// <param name="maxIntensityRange">Maximum value of the range.</param>
    Normalization(const int minIntensityRange = 0, const int maxIntensityRange = 100);

    /// <summary>
    /// Method for data normalization to the given ranges.
    /// </summary>
    /// <param name="intensities">Vector of floating point values representing intensities for each point on the heatmap.</param>
    /// <returns>Vector of normalized floating point values representing intensities for each point on the heatmap.</returns>
    std::vector<double> scaleData(const gsl::span<double> intensities) override;
private:
    const int minIntensityRange, maxIntensityRange, intensityRange;
};
}
