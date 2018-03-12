/*
* DaubechiesFiltersDenoiserTest.cpp
* Tests wavelet denoising using Daubechies filters.
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

#include <gtest/gtest.h>
#include "FloatingPointVectorMatcher.h"
#include "Spectre.libWavelet\DaubechiesFiltersDenoiser.h"
#include "Spectre.libWavelet\WaveletDecomposerRef.h"
#include "Spectre.libWavelet\WaveletReconstructorRef.h"

namespace
{
    using namespace spectre::algorithm::wavelet;
    using namespace spectre;

    TEST(DaubechiesFiltersDenoiserInitialization, initializes)
    {
        EXPECT_NO_THROW(DaubechiesFiltersDenoiser());
    }

    class DaubechiesFiltersDenoiserTest : public ::testing::Test
    {
    public:
        DaubechiesFiltersDenoiserTest()
        {
            signal = {
                0.0,
                1.0,
                0.2,
                0.0,
                3.0,
                6.0,
                0.3,
                7.0,
                0.1,
                0.2,
                0.1
            };
            correctResult = {
                -0.1322,
                -0.0672,
                -0.0567,
                -0.0068,
                0.6509,
                1.2891,
                -1.0844,
                1.7828,
                -0.1743,
                0.0527,
                -0.1567
            };
        }
    protected:
        Data signal;
        Data correctResult;
        DaubechiesFiltersDenoiser denoiser;
    };

    TEST_F(DaubechiesFiltersDenoiserTest, denoises_signal)
    {
        Data denoisedSignal = denoiser.Denoise(signal);
        auto doubleNear = double_near(0.001);
        EXPECT_THAT(correctResult, testing::Pointwise(doubleNear, denoisedSignal));
    }
}
