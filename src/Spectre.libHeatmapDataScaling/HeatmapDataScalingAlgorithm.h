/*
* HeatmapDataScalingAlgorithm.h
* Abstract, base class for scaling data algorithms.
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
#include <span.h>
#include <vector>

namespace Spectre::libHeatmapDataScaling
{
    /// <summary>
    /// Abstract, base class for scaling data algorithms.
    /// </summary>
	class HeatmapDataScalingAlgorithm
	{
	public:
        /// <summary>
        /// Virtual destructor of base HeatmapDataScalingAlgorithm class.
        /// </summary>
		virtual ~HeatmapDataScalingAlgorithm() {};

        /// <summary>
        /// Method for scaling data using different algorithms for contrast enhancement and heatmap blur.
        /// </summary>
        /// <param name="intensities">Vector of floating point values representing intensities for each point on the heatmap.</param>
        /// <returns>Vector of scaled floating point values representing intensities for each point on the heatmap.</returns>
		virtual std::vector<double> scaleData(const gsl::span<double> intensities) = 0;
	};
}
