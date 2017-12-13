/*
* HeatmapDataScalingAlgorithm.h
* Abstract class with scaleData virtual method.
*
Copyright 2017 Daniel Babiak
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
#include "GaussianFilter.h"
#include "gsl.h"

namespace Spectre::libHeatmapDataScaling
{
	class HeatmapDataScalingAlgorithm
	{
	protected:
		GaussianFilter gaussianFilter;
	public:
		virtual ~HeatmapDataScalingAlgorithm() {};
		virtual std::vector<double> scaleData(const gsl::span<double> intensities) = 0;
	};
}
