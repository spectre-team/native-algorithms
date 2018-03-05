/*
* HistogramEqualizationTest.cpp
* Tests HistogramEqualization
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
#include "Spectre.libHeatmapDataScaling/HistogramEqualization.h"

using namespace spectre::visualization;

namespace
{
    class HistogramEqualizationTest : public ::testing::Test
    {
    protected:
        HistogramEqualization histogramEqualization;
        std::vector<double> testIntensities;
        void SetUp() override
        {
            testIntensities = { 3.2, 2.69, 1.19, 1.23, 8.98, 4.15, 4.15, 0.11, 1.76, 8.8 };
        }
    };

    TEST_F(HistogramEqualizationTest, returns_counted_repeating_values)
    {
        std::vector<double> vectorContainingRepeatingValues = { 1.0, 2.0, 1.0, 5.0, 2.0, 1.0 };
        auto result = histogramEqualization.countRepeatingValues(vectorContainingRepeatingValues);
        EXPECT_EQ(result[1], 3);
        EXPECT_EQ(result[2], 2);
        EXPECT_EQ(result[3], 0);
        EXPECT_EQ(result[4], 0);
        EXPECT_EQ(result[5], 1);
    }

    TEST_F(HistogramEqualizationTest, returns_scaled_intensities)
    {
        auto result = histogramEqualization.scaleData(testIntensities);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 142.0, 142.0, 57.0, 57.0, 255.0, 198.0, 198.0, 0.0, 85.0, 255.0 }));
    }

    TEST_F(HistogramEqualizationTest, returns_correct_data_size)
    {
        auto result = histogramEqualization.scaleData(testIntensities);
        EXPECT_EQ(result.size(), testIntensities.size());
    }

    TEST_F(HistogramEqualizationTest, calculates_new_histogram_data)
    {
        std::vector<double> intensitiesForCalculatingNewDataTest = { 0.0, 1.0, 1.0 };
        std::vector<uint8_t> cumulativeDistribution(256, 3);
        cumulativeDistribution[0] = 1;
        cumulativeDistribution[1] = 3;
        auto result = histogramEqualization.calculateNewHistogramData(intensitiesForCalculatingNewDataTest, cumulativeDistribution);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 0.0, 255.0, 255.0 }));
     }
}