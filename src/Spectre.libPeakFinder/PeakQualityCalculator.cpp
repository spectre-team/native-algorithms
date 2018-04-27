/*
* PeakQualityCalculator.cpp
* Calculates qualities of peaks.
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

#include "PeakQualityCalculator.h"


namespace spectre::unsupervised::gmm
{
    Data PeakQualityCalculator::CalculatePeakQualities(const DataView& peaksIntensities, const DataView& valleysIntensities)
    {
        Data peakQualities(peaksIntensities.length());
        for (unsigned i = 0; i < peaksIntensities.length(); ++i)
        {
            DataType maxValleyHeight = 1 + std::max(valleysIntensities[i], valleysIntensities[i + 1]);
            peakQualities[i] = peaksIntensities[i] / maxValleyHeight;
        }
        return peakQualities;
    }
}
