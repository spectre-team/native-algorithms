/*
* SupressionAlgorithm.h
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

#pragma once
#include "Spectre.libHeatmapDataScaling/HeatmapDataScalingAlgorithm.h"

namespace spectre::visualization
{
/// <summary>
/// Class containing suppression algorithm implementation.
/// </summary>
class SuppressionAlgorithm : public HeatmapDataScalingAlgorithm
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="SuppressionAlgorithm"/> class.
    /// </summary>
    /// <param name="topPercent">Percent of outliers which will be suppressed.</param>
    SuppressionAlgorithm(const double topPercent = 0.01);

    /// <summary>
    /// Method for calculating quantile.
    /// </summary>
    /// <param name="probability">Determines the quantile.</param>
    /// <returns>Quantile value.</returns>
    double calculateQuantile(const gsl::span<const double> intensities, const double probability);

    /// <summary>
    /// Method for scaling data using suppression algorithm for contrast enhancement.
    /// </summary>
    /// <param name="intensities">Vector of floating point values representing intensities for each point on the heatmap.</param>
    /// <returns>Vector of scaled floating point values representing intensities for each point on the heatmap.</returns>
    std::vector<double> scaleData(const gsl::span<const double> intensities) override;
private:
    const double topPercent;
};
}
