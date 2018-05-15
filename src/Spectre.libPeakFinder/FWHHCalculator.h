/*
* FWHHCalculator.h
* Class for calculating locations of full width at half height markers for 
* input peak data.
*
Copyright 2018 Dariusz Kuchta

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
    /// Class for calculating locations of full width at half height markers for
    /// input peak data. It calculates it in such manner that it finds both left
    /// and right m / z values of corresponding half - heights to preserve their
    /// positions. Returned containers are aligned, i.e.
    /// rightFWHH[i] - leftFWHH[i] = width for the i'th peak.
    /// </summary>
    class FWHHCalculator
    {
    public:
        /// <summary>
        /// Calculates and returns left m/z values for FWHH markers.
        /// For performance purposes it does not verify the integrity of the data!
        /// Integrity of the data is assumed to be checked before entering the function:
        /// 1. Container x is assumed to be the same size as y.
        /// 2. Container x is assumed to be sorted.
        /// 3. Container of valley indices is assumed to me of size peaks.size() + 1.
        /// 4. Indices of both valleys and peaks must be in range of x and y containers.
        /// 5. Indices of both valleys and peaks are assumed to be reflecting real valleys/peaks.
        /// </summary>
        /// <param name="x">Container view of m/z values.</param>
        /// <param name="y">Container view of intensity values.</param>
        /// <param name="valleys">Container view of valley indices.</param>
        /// <param name="peaks">Container view of peak indices.</param>
        /// <returns>New container of m/z values for left sides of FWHH markers for all peaks.</returns>
        Data GetLeftFWHH(const DataView x, const DataView y, const IndicesView valleys, const IndicesView peaks);
        /// <summary>
        /// Calculates and returns right m/z values for FWHH markers.
        /// For performance purposes it does not verify the integrity of the data!
        /// Integrity of the data is assumed to be checked before entering the function:
        /// 1. Container x is assumed to be the same size as y.
        /// 2. Container x is assumed to be sorted.
        /// 3. Container of valley indices is assumed to me of size peaks.size() + 1.
        /// 4. Indices of both valleys and peaks must be in range of x and y containers.
        /// 5. Indices of both valleys and peaks are assumed to be reflecting real valleys/peaks.
        /// </summary>
        /// <param name="x">Container view of m/z values.</param>
        /// <param name="y">Container view of intensity values.</param>
        /// <param name="valleys">Container view of valley indices.</param>
        /// <param name="peaks">Container view of peak indices.</param>
        /// <returns>New container of m/z values for right sides of FWHH markers for all peaks.</returns>
        Data GetRightFWHH(const DataView x, const DataView y, const IndicesView valleys, const IndicesView peaks);
    private:
        inline Index GetClosestNeighbourIndex(const DataView& xSorted, const DataType value);
        inline DataType LinearInterpolation(DataType x1, DataType x2, DataType y1, DataType y2, DataType x);   //TODO(dkuchta): Maybe in the future move it to libStatistics?
    };
}
