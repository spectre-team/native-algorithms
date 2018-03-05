/*
* SuppressionAlgorithmTest.cpp
* Tests SuppressionAlgorithm
*
Copyright 2018 Daniel Babiak

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
#include <gmock/gmock-matchers.h>
#include "Spectre.libHeatmapDataScaling/SuppressionAlgorithm.h"

using namespace spectre::visualization;

namespace
{
    class SuppressionAlgorithmTest : public ::testing::Test
    {
    public:
        SuppressionAlgorithmTest() : suppressionAlgorithm(), parametrizedSuppressionAlgorithm(0.05) {}
    protected:
        SuppressionAlgorithm suppressionAlgorithm;
        SuppressionAlgorithm parametrizedSuppressionAlgorithm;
        std::vector<double> testIntensities;
        void SetUp() override
        {
            testIntensities = { 3.2, 0.0, 1.23, 1.23 };
        }
    };
    
    TEST_F(SuppressionAlgorithmTest, returns_scaled_intensities)
    {
        auto result = suppressionAlgorithm.scaleData(testIntensities);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 3.1408999999999994, 0.0, 1.2531440033111532, 1.2531440033111532 }));
    }

    TEST_F(SuppressionAlgorithmTest, returns_correct_data_size)
    {
        EXPECT_EQ(suppressionAlgorithm.scaleData(testIntensities).size(), testIntensities.size());
        EXPECT_EQ(parametrizedSuppressionAlgorithm.scaleData(testIntensities).size(), testIntensities.size());
    }

    TEST_F(SuppressionAlgorithmTest, returns_calculated_quantile)
    {
        auto quantile = suppressionAlgorithm.calculateQuantile(testIntensities, 0.99);
        EXPECT_EQ(quantile, 3.1408999999999994);
    }

    TEST_F(SuppressionAlgorithmTest, results_scaled_data_for_given_top_percent)
    {
        auto result = parametrizedSuppressionAlgorithm.scaleData(testIntensities);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 2.9044999999999992, 0.0, 1.3551385780685148, 1.3551385780685148 }));
    }
}