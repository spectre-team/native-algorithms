/*
* WaveletDecomposerRefTest.cpp
* Tests whether the signal is properly decomposed by the reference 
* implementation.
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
#include "Spectre.libWavelet/WaveletDecomposerRef.h"
#include "Spectre.libFunctional/Range.h"
#include "FloatingPointVectorMatcher.h"

namespace
{
using namespace spectre::algorithm::wavelet;
using namespace spectre;

TEST(WaveletDecomposerInitialization, initializes)
{
    EXPECT_NO_THROW(WaveletDecomposerRef());
}

class WaveletDecomposerRefTest : public ::testing::Test
{
public:
    WaveletDecomposerRefTest()
    {
        lastRowLastHighFrequencyResult = {
                -1.00930961719589e-05,
                0.000631661429085606,
                0.00363953257484134,
                -0.0349096779749494,
                0.0939060392824094,
                -0.573668491474043,
                1.48135002884348,
                -1.25125371723593,
                -0.0680747992002441,
                0.371157080651903,
                0.0638308206668811,
                -0.0652726295392059,
                -0.0213226552393410,
                -3.09968872155731e-06,
                0.0
            };
        firstRowLastHighFrequencyResult = {
                0.0,
                -8.84442489411620e-06,
                0.000586138712079808,
                0.00366319641253592,
                -0.0337088162678507,
                0.0882423835249452,
                -0.562277648805622,
                1.47065650410807,
                -1.24839451703031,
                -0.0660665610544188,
                0.370413811253544,
                0.0633207123092398,
                -0.0651742255517986,
                -0.0212504983010336,
                -1.63488447864066e-06
            };
        firstHighFrequencyResult = {
                0,
                -0.230377813308897,
                0.254090943935123,
                0.107678933249283,
                -0.0667168468534168,
                -0.0540778152370236,
                -0.0105974017850692,
                -2.22044604925031e-16,
                -2.22044604925031e-16,
                -8.88178419700125e-16,
                2.30377813308897,
                -4.61430975913130,
                1.21002916292327,
                1.63627886777771,
                -0.0596734693105170,
                -0.380726319282519,
                -0.0953766160656215
            };
        firstLowFrequencyResult = {
                0.0,
                -4.06844404044861e-07,
                2.69624376778159e-05,
                0.000170805548639498,
                -0.00170291259939498,
                0.00309960587361391,
                -0.0165956761940691,
                0.0479600502210081,
                0.0561611188212727,
                -0.339939690673109,
                -0.0487037936781232,
                1.23299325679310,
                1.41702876032074,
                0.461966379081157,
                3.55408918941385e-05
            };
        lastLowFrequencyResult = {
                -4.64283404088808e-07,
                2.90564870809657e-05,
                0.000170041467736160,
                -0.00176998109340235,
                0.00336519663747599,
                -0.0167679988715969,
                0.0469668204068491,
                0.0577080776967957,
                -0.338551325051386,
                -0.0523643871799257,
                1.23076472595957,
                1.41934784856588,
                0.463535004863161,
                6.73843951653069e-05,
                0.0
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

TEST_F(WaveletDecomposerRefTest, decomposes_the_signal)
{
    Data signal = spectre::core::functional::range<DataType>(10);
    WaveletCoefficients coefficients = decomposer.Decompose(std::move(signal));

    // Due to high amount of coefficients generated, only a few sets will be compared
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
