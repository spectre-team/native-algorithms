/*
* ScalingDataUtils.h
* Utils for scaling data.
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

#include <gsl/span>
#include <vector>

namespace spectre::visualization
{
/// <summary>
/// Method for data scaling data to the given ranges.
/// </summary>
/// <param name="intensities">Vector of floating point values representing data for scaling.</param>
/// <returns>Vector of floating point values representing scaled data.</returns>
std::vector<double> scaleDataToRange(const gsl::span<double> intensities, const int minIntensityRange = std::numeric_limits<uint8_t>::min(), const int maxIntensityRange = std::numeric_limits<uint8_t>::max());
}
