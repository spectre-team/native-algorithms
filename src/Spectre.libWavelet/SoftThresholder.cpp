/*
 * SoftThresholder.cpp
 * Performs a soft tresholding on the provided signal.
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
#include "Spectre.libFunctional/Transform.h"
#include "SoftThresholder.h"

namespace spectre::algorithm::wavelet
{
SoftThresholder::SoftThresholder(DataType threshold)
    : m_Threshold(threshold)
{
}

Signal SoftThresholder::operator()(const Signal &signal) const
{
    const auto& threshold = m_Threshold;
    const auto thresholding = [threshold](DataType value)
    {
        return std::max(static_cast<DataType>(0), std::abs(value) - threshold) * (value < 0 ? -1 : 1);
    };
    return core::functional::transform(gsl::as_span(signal), thresholding);
}
}
