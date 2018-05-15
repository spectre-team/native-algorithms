/*
 * ExtremaFinder.h
 * Finding indices of extrema in signals.
 *
 Copyright 2018 Michal Gallus
 
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
#include <vector>

namespace spectre::algorithm::peakfinder
{
/// <summary>
/// Finds extrema in the signal.
/// </summary>
class ExtremaFinder
{
public:
    /// <summary>
    /// Finds indices of minima of the signal.
    /// </summary>
    /// <param name="signal">Signal in which minima shall be found.</param>
    /// <returns>Indices of minima in the signal.</returns>
    Indices FindValleys(const DataView& signal) const;
    /// <summary>
    /// Finds indices of maxima of the signal.
    /// </summary>
    /// <param name="signal">Signal in which maxima shall be found.</param>
    /// <returns>Indices of maxima in the signal.</returns>
    Indices FindPeaks(const DataView& signal) const;
};
}
