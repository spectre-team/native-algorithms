/*
 * Convolution.h
 * Performs convolution over a 1D signal.
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
#include <span.h>
#include "DataTypes.h"

namespace spectre::algorithm::wavelet
{
/// <summary>
/// Filters the signal using rational transfer function.
/// The denominator is set to 1.
/// </summary>
class Convolution
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Convolution"/> class.
    /// </summary>
    explicit Convolution();
    /// <summary>
    /// Convolves the signal using provided kernel.
    /// </summary>
    /// <param name="kernel">Kernel to be used.</param>
    /// <param name="signal">Signal to be convolved.</param>
    /// <returns>Filtered signal.</returns>
    Signal Convolve(const Signal& kernel, const Signal& signal);
};
}
