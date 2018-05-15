/*
* DynamicProgramminInitializationTestcpp.cpp
* Provides implementation of tests for the purpose of testing
* functions performing dynamic programming initialization of 
* gmms.
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
#define GTEST_LANG_CXX11 1

#include <gtest/gtest.h>
#include "Spectre.libGaussianMixtureModelling/DynamicProgrammingInitialization.h"

namespace spectre::unsupervised::gmm
{
class DynamicProgrammingInitializationTest : public ::testing::Test
{
public:
    DynamicProgrammingInitializationTest()
    {
        spectrum.mzs = { 1.0, 2.0, 3.0, 4.0 };
        spectrum.intensities = { 5.0, 3.0, 4.0, 2.0 };
    }

protected:
    Spectrum spectrum;
    static constexpr DataType MAX_ERROR = 0.0001;
};

TEST_F(DynamicProgrammingInitializationTest, quality_one_function_returns_corr_result)
{
    DataType intensitySum = std::accumulate(spectrum.intensities.begin(),
        spectrum.intensities.end(), 0.0);
    DataType quality = ComputeBasicQuality(spectrum, intensitySum);
    EXPECT_NEAR(1.1683, quality, MAX_ERROR);
}

TEST_F(DynamicProgrammingInitializationTest, quality_function_returns_inf_on_small_heights)
{
    Data smallIntensities = { 0.00001, 0.00001, 0.00001, 0.00001 };
    Spectrum blockData;
    blockData.mzs = spectrum.mzs;
    blockData.intensities = smallIntensities;
    DataType quality = ComputeBlockQuality(blockData, 0.5, 0.4635);
    EXPECT_EQ(std::numeric_limits<DataType>::infinity(), quality);
}

TEST_F(DynamicProgrammingInitializationTest, quality_function_returns_correct_result)
{
    DataType quality = ComputeBlockQuality(spectrum, 0.5, 0.4635);
    EXPECT_NEAR(0.5269, quality, MAX_ERROR);
}

TEST_F(DynamicProgrammingInitializationTest, optimal_blocks_return_optimal_result)
{
    Spectrum longSpectrum;
    longSpectrum.mzs = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    longSpectrum.intensities = { 5.0, 8.0, 2.0, 4.0, 2.0, 3.0, 4.0, 3.0, 7.0 };
    SpectrumView view = longSpectrum;
    const unsigned mzCount = (unsigned)view.mzs.size();
    Matrix<DataType> qualityOfRange(mzCount, mzCount);
    for (unsigned i = 0; i < mzCount - 1; i++)
    {
        for (unsigned j = i + 1; j < mzCount; j++)
        {
            qualityOfRange[i][j] = ComputeBlockQuality(view.subspan(i, j - i), 0.5, 0.4635);
        }
    }
    Indices partitions = ComputeOptimalBlocks(view, 3, qualityOfRange, 0.5, 0.4635);
    Indices correctResult = { 0u, 3u, 5u, 7u, 8u };
    ASSERT_EQ(partitions, correctResult);
}

TEST_F(DynamicProgrammingInitializationTest, optimal_blocks_throw_on_invalid_data)
{
    const unsigned mzCount = (unsigned)spectrum.mzs.size();
    Matrix<DataType> qualityOfRange(mzCount, mzCount);
    EXPECT_THROW(ComputeOptimalBlocks(spectrum, 3, qualityOfRange, 0.5, 0.4635), OutOfRangeException);
    EXPECT_THROW(ComputeOptimalBlocks(spectrum, 1, qualityOfRange, -0.5, 0.4635), std::invalid_argument);
    EXPECT_THROW(ComputeOptimalBlocks(spectrum, 1, qualityOfRange, 0.5, -0.4635), std::invalid_argument);
}
}
