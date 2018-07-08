/*
* DynamicProgramminInitializationTest.cpp
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
        longSpectrum.mzs = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
        longSpectrum.intensities = { 5.0, 8.0, 2.0, 4.0, 2.0, 3.0, 4.0, 3.0, 7.0 };
    }

protected:
    Spectrum spectrum;
    Spectrum longSpectrum;
    static constexpr DataType MAX_ERROR = 0.0001;
};

TEST_F(DynamicProgrammingInitializationTest, correctly_computes_mz_range)
{
    DataType range = ComputeMzRange(spectrum.mzs);
    EXPECT_NEAR(3.0, range, MAX_ERROR);
}

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

TEST_F(DynamicProgrammingInitializationTest, computes_quality_matrix_correctly)
{
    SpectrumView view = spectrum;
    const unsigned mzCount = (unsigned)spectrum.mzs.size();
    const unsigned numOfBlocks = 2;
    const DataType delta = 0.5;
    const DataType minStd = 0.4635;
    Matrix<DataType> qualities = std::move(ComputeQualityMatrix(spectrum, delta, minStd));

    // set correct result
    Matrix<DataType> correct(mzCount, mzCount);
    DataType infinity = std::numeric_limits<DataType>::infinity();
    correct[0][1] = infinity;
    correct[0][2] = ComputeBlockQuality(view.subspan(0, 2), delta, minStd);
    correct[0][3] = ComputeBlockQuality(view.subspan(0, 3), delta, minStd);
    correct[1][2] = infinity;
    correct[1][3] = ComputeBlockQuality(view.subspan(1, 2), delta, minStd);
    correct[2][3] = infinity;

    // compare the matrices
    ASSERT_TRUE(qualities.Equals(correct, mzCount * mzCount));
}

TEST_F(DynamicProgrammingInitializationTest, computes_initial_components_correctly)
{
    const unsigned mzCount = (unsigned)longSpectrum.mzs.size();
    const unsigned numOfBlocks = 2;
    const DataType delta = 0.5;
    const DataType minStd = 0.4635;
    Matrix<DataType> qualityOfRange = std::move(ComputeQualityMatrix(longSpectrum, delta, minStd));
    Indices partitions = ComputeOptimalBlocks(longSpectrum, numOfBlocks, qualityOfRange, delta, minStd);
    std::vector<GaussianComponent> components = ComputeGaussiansFromBlocks(longSpectrum, partitions);

    EXPECT_NEAR(components[0].mean, 0.8, MAX_ERROR);
    EXPECT_NEAR(components[0].weight, 0.39473, MAX_ERROR);
    EXPECT_NEAR(components[0].deviation, 1.0, MAX_ERROR);
    EXPECT_NEAR(components[1].mean, 3.88888, MAX_ERROR);
    EXPECT_NEAR(components[1].weight, 0.23684, MAX_ERROR);
    EXPECT_NEAR(components[1].deviation, 1.0, MAX_ERROR);
}

TEST_F(DynamicProgrammingInitializationTest, class_computes_initial_components_correctly)
{
    const unsigned mzCount = (unsigned)longSpectrum.mzs.size();
    const unsigned numOfComponents = 2;
    const DataType delta = 0.5;
    const DataType minStd = 0.4635;
    DynProgInitialization initialization(longSpectrum, delta, minStd);
    GaussianMixtureModel components = initialization.Initialize(longSpectrum, numOfComponents);

    EXPECT_NEAR(components[0].mean, 0.8, MAX_ERROR);
    EXPECT_NEAR(components[0].weight, 0.39473, MAX_ERROR);
    EXPECT_NEAR(components[0].deviation, 1.0, MAX_ERROR);
    EXPECT_NEAR(components[1].mean, 3.88888, MAX_ERROR);
    EXPECT_NEAR(components[1].weight, 0.23684, MAX_ERROR);
    EXPECT_NEAR(components[1].deviation, 1.0, MAX_ERROR);
}
}
