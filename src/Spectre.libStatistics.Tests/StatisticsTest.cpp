/*
* StatisticsTest.cpp
* Tests basic vector statistics.
*
Copyright 2018 Grzegorz Mrukwa, Michal Gallus

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
#include "Spectre.libStatistics/Statistics.h"
#include "Spectre.libStatistics/AlignedAllocator.h"
#include "Spectre.libFunctional/Range.h"

namespace
{
using namespace testing;
using namespace spectre::statistics::simple_statistics;

using Data = const std::vector<double>;

Data dataStorage { 1,2,3 };
auto data = gsl::make_span(dataStorage);
Data emptyStorage {};
auto empty = gsl::make_span(emptyStorage);
typedef std::vector<float, aligned_allocator<float, sizeof(__m256)>> AlignedFloatVector;
AlignedFloatVector bigFloatData =
    spectre::core::functional::range<float, aligned_allocator<float, sizeof(__m256)>>(0.0f, 1000000.0f, 1.0f);
typedef std::vector<double, aligned_allocator<double, sizeof(__m256)>> AlignedDoubleVector;
AlignedDoubleVector bigDoubleData =
spectre::core::functional::range<double, aligned_allocator<double, sizeof(__m256)>>(0.0, 1000000.0, 1.0);

// Used for the purposes of benchmarking
TEST(SumTest, DISABLED_calculates_vector_sum2)
{
    constexpr float MAX_ERROR = 59146241.0f;
    ASSERT_NEAR(Sum<float>(bigFloatData), 499999500000.0f, MAX_ERROR);
}

TEST(SumTest, calculates_vector_sum)
{
    EXPECT_THAT(Sum(data), 6.);
}

TEST(SumTest, sum_of_empty_is_zero)
{
    EXPECT_THAT(Sum(empty), 0.);
}

TEST(AccurateSumTest, calculates_float_vector_sum)
{
    constexpr float MAX_ERROR = 0.0000001f;
    ASSERT_NEAR(AccurateSum(bigFloatData), 499999500000.0f, MAX_ERROR);
}

TEST(AccurateSumTest, calculates_double_vector_sum)
{
    constexpr double MAX_ERROR = 0.0000001;
    ASSERT_NEAR(AccurateSum(bigDoubleData), 499999500000.0, MAX_ERROR);
}

TEST(AccurateSumTest, sum_of_empty_is_zero)
{
    ASSERT_EQ(AccurateSum(empty), 0.0);
}

TEST(MeanTest, calculates_vector_mean)
{
    EXPECT_THAT(Mean(data), 2.);
}

TEST(MeanTest, mean_of_empty_is_zero)
{
    EXPECT_THAT(Mean(empty), 0.);
}

TEST(MedianTest, calculates_vector_median)
{
    EXPECT_THAT(Median(data), 2.);
}

TEST(MedianTest, median_of_empty_is_zero)
{
    EXPECT_THAT(Median(empty), 0.);
}

TEST(VarianceTest, calculates_unbiased_vector_variance_by_default)
{
    EXPECT_THAT(Variance(data), DoubleEq(1.0));
}

TEST(VarianceTest, calculates_biased_vector_variance_on_demand)
{
    EXPECT_THAT(Variance(data, false), DoubleEq(2. / 3.));
}

TEST(VarianceTest, variance_of_empty_is_zero)
{
    EXPECT_THAT(Variance(empty), DoubleEq(0.));
    EXPECT_THAT(Variance(empty, false), DoubleEq(0.));
}

TEST(TwoSampleVariance, variance_of_but_differing_internally_homogeneous_samples_is_zero)
{
    const Data first { 1., 1., 1. };
    const Data second { 2., 2., 2., 2. };
    EXPECT_THAT(Variance(gsl::make_span(first), gsl::make_span(second)), DoubleEq(0.));
}

TEST(TwoSampleVariance, evaluates_to_proper_value)
{
    const Data first { 1., 2., 3. };
    const Data second { 4., 6., 8., 10., 12. };
    const auto two_sample_variance = Variance(gsl::make_span(first), gsl::make_span(second));
    const auto expected_variance = ((1.0 + 0.0 + 1.0) * 3.0 / 2.0 + (16.0 + 4.0 + 0.0 + 4.0 + 16.0) * 5.0 / 4.0) / (3. + 5. - 2.);
    EXPECT_THAT(two_sample_variance, DoubleEq(expected_variance));
}

TEST(MeanAbsoluteDeviationTest, calculates_mean_absolute_deviation_of_vector)
{
    EXPECT_THAT(MeanAbsoluteDeviation(data), DoubleEq(2. / 3.));
}

TEST(MeanAbsoluteDeviationTest, mean_absolute_deviation_of_empty_is_zero)
{
    EXPECT_THAT(MeanAbsoluteDeviation(empty), 0.);
}

TEST(MedianAbsoluteDeviationTest, calculates_median_absolute_deviation_of_vector)
{
    EXPECT_THAT(MedianAbsoluteDeviation(data), DoubleEq(1.0));
}

TEST(MedianAbsoluteDeviationTest, median_absolute_deviation_of_empty_is_zero)
{
    EXPECT_THAT(MedianAbsoluteDeviation(empty), 0.);
}
}
