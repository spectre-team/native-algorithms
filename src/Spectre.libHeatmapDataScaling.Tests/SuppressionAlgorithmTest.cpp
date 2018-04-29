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
        std::vector<double> expected = { 3.14, 0.0, 1.25, 1.25 };
        for (auto i = 0; i < 4; i++)
        {
            EXPECT_NEAR(result[i], expected[i], 2);
        }
    }

    TEST_F(SuppressionAlgorithmTest, returns_correct_data_size)
    {
        EXPECT_EQ(suppressionAlgorithm.scaleData(testIntensities).size(), testIntensities.size());
        EXPECT_EQ(parametrizedSuppressionAlgorithm.scaleData(testIntensities).size(), testIntensities.size());
    }

    TEST_F(SuppressionAlgorithmTest, returns_calculated_quantile)
    {
        auto quantile = suppressionAlgorithm.calculateQuantile(testIntensities, 0.99);
        EXPECT_NEAR(quantile, 3.14, 2);
    }

    TEST_F(SuppressionAlgorithmTest, results_scaled_data_for_given_top_percent)
    {
        auto result = parametrizedSuppressionAlgorithm.scaleData(testIntensities);
        std::vector<double> expected = { 2.90, 0.0, 1.35, 1.35 };
        for(auto i = 0; i < 4; i++)
        {
            EXPECT_NEAR(result[i], expected[i], 2);
        }
    }
}