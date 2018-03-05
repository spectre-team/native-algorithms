/*
* ScalingDataUtilsTest.cpp
* Tests ScalingDataUtils
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
#include "Spectre.libHeatmapDataScaling/ScalingDataUtils.h"

using namespace spectre::visualization;

namespace
{
    class ScalingDataUtilsTest : public ::testing::Test
    {
    protected:
        std::vector<double> testIntensities;
        void SetUp() override
       {
           testIntensities = { 3.2, 2.69, 1.23, 1.23 };
       }
    };

    TEST_F(ScalingDataUtilsTest, returns_normalized_data)
    {
        auto result = normalizeData(testIntensities);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 255.0, 188.98477157360404, 0.0, 0.0 }));
    }

    TEST_F(ScalingDataUtilsTest, returns_correct_data_size)
    {
        EXPECT_EQ(normalizeData(testIntensities).size(), testIntensities.size());
        EXPECT_EQ(normalizeData(testIntensities).size(), testIntensities.size());
    }

    TEST_F(ScalingDataUtilsTest, results_normalized_data_for_min_max_parameters)
    {
        auto result = normalizeData(testIntensities, 0, 100);
        EXPECT_THAT(result, testing::ContainerEq(std::vector<double>{ 100.0, 74.111675126903549, 0.0, 0.0 }));
    }

    TEST_F(ScalingDataUtilsTest, results_data_with_correct_min_max_value)
    {
        auto result = normalizeData(testIntensities);
        double const min = *min_element(std::begin(result), std::end(result));
        double const max = *max_element(std::begin(result), std::end(result));
        EXPECT_EQ(min, 0.0);
        EXPECT_EQ(max, 255.0);
    }
}