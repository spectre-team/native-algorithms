/*
 * Convolution.cpp
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
#include "Convolution.h"

namespace spectre::algorithm::wavelet
{
Convolution::Convolution()
{
}

Data Convolution::Convolve(const Kernel& kernel, const Data &signal) const
{
    return Convolve(kernel, signal, signal.size());
}

Data Convolution::Convolve(const Kernel& kernel, const Data &signal, size_t length) const
{
    Data convolved(length);
    for (unsigned n = 0u; n < length; ++n)
    {
        size_t limit = kernel.size() < (n + 1) ? kernel.size() : (n + 1);
        DataType result = 0.0f; // @sand3r-: speeds the computations up on vc++
        for (unsigned i = 0u; i < limit; ++i)
        {
            result += kernel[i] * signal[n - i];
        }
        convolved[n] = result;
    }
    return convolved;
}
}
