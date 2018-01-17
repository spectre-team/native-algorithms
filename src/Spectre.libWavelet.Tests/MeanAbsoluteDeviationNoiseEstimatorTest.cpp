/*
* MeanAbsoluteDeviationNoiseEstimatorTest.cpp
* Tests Mean Absolute Deviation noise estimator.
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
#include "Spectre.libWavelet\MeanAbsoluteDeviationNoiseEstimator.h"

namespace
{
    using namespace spectre::algorithm::wavelet;

    TEST(MeanAbsoluteDeviationNoiseEstimatorInitialization, initializes)
    {
        EXPECT_NO_THROW(MeanAbsoluteDeviationNoiseEstimator(5.0));
        EXPECT_NO_THROW(MeanAbsoluteDeviationNoiseEstimator());
    }

    class MeanAbsoluteDeviationNoiseEstimatorTest : public ::testing::Test
    {
    protected:
        MeanAbsoluteDeviationNoiseEstimator estimator;
    };

    TEST_F(MeanAbsoluteDeviationNoiseEstimatorTest, estimate_noise)
    {
        Signal signal = { 1.0f, 2.0f, 3.0f };
        constexpr DataType result = static_cast<DataType>(1.4650890114825907);
        DataType estimate = estimator.Estimate(signal);
        ASSERT_EQ(estimate, result);
    }
}
