/*
* PeakQualityCalculator.h
* Calculates qualities of peaks.
*
Copyright 2018 Spectre Team

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
#include "Common/DataTypes.h"


namespace spectre::algorithm::peakfinder
{
    /// <summary>
    /// Calculates qualities of peaks.
    /// </summary>
    class PeakQualityCalculator
    {
    public:
        /// <summary>
        /// Determines qualities of peaks.
        /// </summary>
        /// <param name="peaksIntensities">Container of intensities of peaks.</param> // TODO Adjust this
        /// <param name="valleysIntensities">Container of intensities of valleys.</param>
        /// <returns>Container of qualities for peaks, index-aligned with container of peak intensities.</returns>
        Data CalculatePeakQualities(const DataView& peaksIntensities, const DataView& valleysIntensities);
    };
}
