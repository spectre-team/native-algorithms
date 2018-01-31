/*
* HistogramEqualization.h
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

#pragma once
#include <map>
#include "Spectre.libHeatmapDataScaling/HeatmapDataScalingAlgorithm.h"

namespace spectre::visualization
{
/// <summary>
/// Class containing histogram equalization algorithm implementation.
/// </summary>
class HistogramEqualization : public HeatmapDataScalingAlgorithm
{
public:
    /// <summary>
    /// Method for scaling data using histogram equalization algorithm for contrast enhancement.
    /// </summary>
    /// <param name="intensities">Vector of floating point values representing intensities for each point on the heatmap.</param>
    /// <returns>Vector of scaled floating point values representing intensities for each point on the heatmap.</returns>
    std::vector<double> scaleData(const gsl::span<double> intensities) override;
    /// <summary>
    /// Method for rounding vector of floating point values.
    /// </summary>
    /// <param name="intensities">Vector of floating point values of intensities.</param>
    /// <returns>Vector of rounded intensities.</returns>
    std::vector<double> HistogramEqualization::roundIntensities(const gsl::span<double> intensities);
    /// <summary>
    /// Method for counting repeating values in the vector.
    /// </summary>
    /// <param name="intensities">Vector of floating point values of intensities.</param>
    /// <returns>Map in which key represents intensity value while value stores the number of occurrences of this intensity in the input vector.</returns>
    std::map<double, unsigned int> countRepeatingValues(const gsl::span<double> intensities);
    /// <summary>
    /// Method for calculating cumulative distribution.
    /// </summary>
    /// <param name="histogramMap">Map containing intensity value as a key and the number of occurrences in the input vector as a value .</param>
    /// <returns>Map in which key represents intensity value while value stores cumulative distribution.</returns>
    std::map<double, unsigned int> calculateCumulativeDistribution(std::map<double, unsigned int> histogramMap);
    /// <summary>
    /// Method for calculating new histogram data based on input vector of intensities and cumulative distribution.
    /// </summary>
    /// <param name="intensities">Vector of floating point values of intensities.</param>
    /// <param name="histogramMap">Map containing intensity value as a key and cumulative distribution as a value.</param>
    /// <returns>Vector of scaled floating point values representing intensities for each point on the heatmap.</returns>
    std::vector<double> calculateNewHistogramData(const gsl::span<double> intensities, std::map<double, unsigned int> histogramMap);
};
}