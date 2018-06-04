/*
* GaussianDistributionTest.cpp
* Provides implementation of tests for the purpose of testing
* Gaussian Mixture Modelling.
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
#include <algorithm>
#include <stdlib.h>
#include "Spectre.libGaussianMixtureModelling/GaussianDistribution.h"

namespace spectre::unsupervised::gmm
{
    class GaussianDistributionTest : public ::testing::Test
    {
    protected:
        static constexpr DataType MAX_ERROR = 0.00001;
        DataType sample = 1.0;
        DataType mean = 1.5;
        DataType standardDeviation = 2.5;
    };

    TEST_F(GaussianDistributionTest, function_computes_probability_correctly)
    {
        DataType result = Gaussian(sample, mean, standardDeviation);
        DataType correct = 0.15641707759018236;
        EXPECT_NEAR(correct, result, MAX_ERROR);
    }

    TEST_F(GaussianDistributionTest, method_computes_probability_correctly)
    {
        GaussianDistribution dist(mean, standardDeviation);
        DataType result = dist.Compute(sample);
        DataType correct = 0.15641707759018236;
        EXPECT_NEAR(correct, result, MAX_ERROR);
    }
}
