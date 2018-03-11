/*
* SoftThresholderTest.cpp
* Testing of the soft tresholding.
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
#include "Spectre.libFunctional\Range.h"
#include "Spectre.libWavelet\SoftThresholder.h"
#include "Spectre.libWavelet\WaveletDecomposerRef.h"
#include "Spectre.libWavelet\MedianAbsoluteDeviationNoiseEstimator.h"

namespace
{
    using namespace spectre::algorithm::wavelet;
    using namespace spectre;

    TEST(SoftThresholderInitialization, initializes)
    {
        EXPECT_NO_THROW(SoftThresholder(0.0));
        EXPECT_NO_THROW(SoftThresholder(9.0));
        EXPECT_NO_THROW(SoftThresholder(-11.0));
    }

    class SoftThresholderTest : public ::testing::Test
    {
    public:
        SoftThresholderTest()
        {
            firstHighFrequencyResult = {
                0,
                -0.127493589089253,
                0.151206719715479,
                0.00479470902963920,
                0,
                0,
                0,
                0,
                0,
                0,
                2.20089390886932,
                -4.51142553491165,
                1.10714493870363,
                1.53339464355807,
                0,
                -0.277842095062875,
                0
            };
            firstRowLastHighFrequencyResult = {
                0,
                0,
                0,
                0,
                0,
                0,
                -0.459393424585978,
                1.36777227988842,
                -1.14551029281067,
                0,
                0.267529587033900,
                0,
                0,
                0,
                0
            };
            lastRowLastHighFrequencyResult = {
                0,
                0,
                0,
                0,
                0,
                -0.470784267254399,
                1.37846580462384,
                -1.14836949301628,
                0,
                0.268272856432259,
                0,
                0,
                0,
                0,
                0
            };
            firstLowFrequencyResult = {
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                -0.237055466453466,
                0,
                1.13010903257345,
                1.31414453610110,
                0.359082154861514,
                0,
            };
            lastLowFrequencyResult = {
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                -0.235667100831742,
                0,
                1.12788050173993,
                1.31646362434624,
                0.360650780643517,
                0,
                0,
            };
        }
    protected:
        WaveletDecomposerRef decomposer;
        Data lastRowLastHighFrequencyResult;
        Data firstRowLastHighFrequencyResult;
        Data firstHighFrequencyResult;
        Data firstLowFrequencyResult;
        Data lastLowFrequencyResult;
    };

    TEST_F(SoftThresholderTest, properly_tresholds_coefficients)
    {
        constexpr unsigned signalLength = 10;
        Data signal = spectre::core::functional::range<DataType>(signalLength);

        // Decompose signal
        WaveletCoefficients coefficients = decomposer.Decompose(std::move(signal));

        // Extract coefficients necessary for noise estimator
        Data highFreqCoefficients(signalLength);
        highFreqCoefficients.assign(coefficients.data[0][0].begin(),
            coefficients.data[0][0].begin() + signalLength);

        // Estimate tresholder's treshold value based on extracted coefficients
        MedianAbsoluteDeviationNoiseEstimator noiseEstimator;
        DataType treshold = noiseEstimator.Estimate(highFreqCoefficients);
        SoftThresholder tresholder(treshold);

        // Appply Soft-tresholding
        WaveletCoefficients output = tresholder(std::move(coefficients));
        WaveletCoefficients correctOutput;

        // Check if the coefficients have appropriate values
        auto doubleNear = double_near(0.001);
        EXPECT_THAT(coefficients.data[0][0],
            testing::Pointwise(doubleNear, firstHighFrequencyResult));
        EXPECT_THAT(coefficients.data[WAVELET_LEVELS - 1][0],
            testing::Pointwise(doubleNear, firstRowLastHighFrequencyResult));
        EXPECT_THAT(coefficients.data[WAVELET_LEVELS - 1][(1 << (WAVELET_LEVELS - 1)) - 1],
            testing::Pointwise(doubleNear, lastRowLastHighFrequencyResult));
        EXPECT_THAT(coefficients.data[WAVELET_LEVELS][0],
            testing::Pointwise(doubleNear, firstLowFrequencyResult));
        EXPECT_THAT(coefficients.data[WAVELET_LEVELS][(1 << (WAVELET_LEVELS - 1)) - 1],
            testing::Pointwise(doubleNear, lastLowFrequencyResult));
    }
}
