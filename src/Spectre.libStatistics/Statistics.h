/*
* Statistics.h
* Statistics calculated on vectors.
*
Copyright 2017-2018 Grzegorz Mrukwa, Michal Gallus

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

#pragma once
#include <cstring>
#include <immintrin.h>
#include <numeric>
#include "Spectre.libStatistics/Math.h"


namespace spectre::statistics::simple_statistics
{
static float horizontal_sum_ps(__m128 vector)
{
    __m128 shuffled = _mm_movehdup_ps(vector);
    __m128 sums = _mm_add_ps(vector, shuffled);
    shuffled = _mm_movehl_ps(shuffled, sums);
    sums = _mm_add_ss(sums, shuffled);
    return _mm_cvtss_f32(sums);
}

static double horizontal_sum_pd(__m128d vector)
{
    __m128 undefined = _mm_undefined_ps();
    __m128 tempShuffled = _mm_movehl_ps(undefined, _mm_castpd_ps(vector));
    __m128d shuffled = _mm_castps_pd(tempShuffled);
    return  _mm_cvtsd_f64(_mm_add_sd(vector, shuffled));
}

/// <summary>
/// Sum the specified data taking into account floating point innacuracy.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Sum of the elements</returns>
inline float AccurateSum(gsl::span<const float> data)
{
    constexpr unsigned NUMBERS_PER_VEC = 8;
    static float remains[NUMBERS_PER_VEC];
    memset(remains, 0, sizeof(float) * NUMBERS_PER_VEC);
    __m256 sums = _mm256_setzero_ps();
    __m256 corrections = _mm256_setzero_ps();
    const size_t dataSize = data.size();  // Discard reminder of size/8 div
    const size_t maxVecSize = dataSize & ~(NUMBERS_PER_VEC - 1);
    unsigned i;
    for (i = 0; i < maxVecSize; i += NUMBERS_PER_VEC)
    {
        const __m256 dataVector = _mm256_load_ps(&data[i]);
        const __m256 corrected = _mm256_sub_ps(dataVector, corrections);
        const __m256 newSums = _mm256_add_ps(sums, corrected);
        corrections = _mm256_sub_ps(_mm256_sub_ps(newSums, sums), corrected);
        sums = newSums;
    }
    memcpy(remains, data.data() + i, (dataSize - i) * sizeof(float));
    const __m256 remainsVector = _mm256_load_ps(remains);
    const __m256 corrected = _mm256_sub_ps(remainsVector, corrections);
    sums = _mm256_add_ps(sums, corrected);

    __m128 lowFloats = _mm256_castps256_ps128(sums);
    const __m128 highFloats = _mm256_extractf128_ps(sums, 1); // high 128
    lowFloats = _mm_add_ps(lowFloats, highFloats);
    return horizontal_sum_ps(lowFloats);
}

/// <summary>
/// Sum the specified data taking into account floating point innacuracy.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Sum of the elements</returns>
inline double AccurateSum(gsl::span<const double> data)
{
    constexpr unsigned NUMBERS_PER_VEC = 4;
    static double remains[NUMBERS_PER_VEC];
    memset(remains, 0, sizeof(double) * NUMBERS_PER_VEC);
    __m256d sums = _mm256_setzero_pd();
    __m256d corrections = _mm256_setzero_pd();
    const size_t dataSize = data.size();  // Discard reminder of size/8 div
    const size_t maxVecSize = dataSize & ~(NUMBERS_PER_VEC - 1);
    unsigned i;
    for (i = 0; i < maxVecSize; i += NUMBERS_PER_VEC)
    {
        const __m256d dataVector = _mm256_load_pd(&data[i]);
        const __m256d corrected = _mm256_sub_pd(dataVector, corrections);
        const __m256d newSums = _mm256_add_pd(sums, corrected);
        corrections = _mm256_sub_pd(_mm256_sub_pd(newSums, sums), corrected);
        sums = newSums;
    }
    memcpy(remains, data.data() + i, (dataSize - i) * sizeof(double));
    const __m256d remainsVector = _mm256_load_pd(remains);
    const __m256d corrected = _mm256_sub_pd(remainsVector, corrections);
    sums = _mm256_add_pd(sums, corrected);

    __m128d lowDoubles = _mm256_castpd256_pd128(sums);
    const __m128d highDoubles = _mm256_extractf128_pd(sums, 1); // high 128
    lowDoubles = _mm_add_pd(lowDoubles, highDoubles);
    return horizontal_sum_pd(lowDoubles);
}

/// <summary>
/// Sum the specified data.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Sum of the elements</returns>
template <class DataType>
constexpr DataType Sum(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return std::accumulate(data.begin(), data.end(), static_cast<DataType>(0));
}

/// <summary>
/// Normalize a given vector.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Normalized elements.</returns>
template <class DataType>
constexpr std::vector<DataType> Normalize(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return basic_math::divideBy(data, Sum(data));
}

/// <summary>
/// Calculate mean of the specified data.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Mean of the elements</returns>
template <class DataType>
constexpr DataType Mean(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return Sum(data) / (data.size() != 0 ? data.size() : 1);
}

/// <summary>
/// Obtain median of the specified data. Return 0 when the input data is empty.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Median of the elements.</returns>
template <class DataType>
DataType Median(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    if (data.size() == 0) return static_cast<DataType>(0.0);
    std::vector<typename std::remove_const<DataType>::type> sortedData(data.begin(), data.end());
    std::sort(sortedData.begin(), sortedData.end());
    if (data.size() % 2) return sortedData[data.size() / 2];
    else return (sortedData[data.size() / 2] + sortedData[data.size() / 2 - 1]) / 2.0;
}

/// <summary>
/// Calculate variance the specified data.
/// </summary>
/// <param name="data">The data.</param>
/// <param name="unbiased">if set to <c>true</c>, unbiased estimator is obtained, biased otherwise.</param>
/// <returns>Sample variance</returns>
template <class DataType>
DataType Variance(gsl::span<const DataType> data, bool unbiased = true)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    const auto mean = Mean(data);
    const auto differences = basic_math::minus<DataType>(data, mean);
    const auto squaredDifferences = basic_math::multiplyBy<DataType>(differences, differences);
    const auto normalizationFactor = static_cast<DataType>(data.size()) - static_cast<DataType>(unbiased);
    return Sum<DataType>(squaredDifferences) / (normalizationFactor == 0 ? static_cast<DataType>(1) : normalizationFactor);
}

/// <summary>
/// Calculate variance in two independent samples.
/// </summary>
/// <param name="first">The first sample.</param>
/// <param name="second">The second sample.</param>
/// <returns>Two-samples variance.</returns>
template <class DataType>
DataType Variance(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    const auto weightedFirst = static_cast<DataType>(first.size()) * Variance(first);
    const auto weightedSecond = static_cast<DataType>(second.size()) * Variance(second);
    const auto normalizationConstant = static_cast<DataType>(first.size() + second.size()) - static_cast<DataType>(2.);
    return (weightedFirst + weightedSecond) / normalizationConstant;
}

/// <summary>
/// Find mean absolute deviation of the data.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Mean absolute deviation of the elements.</returns>
template <class DataType>
DataType MeanAbsoluteDeviation(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    const auto mean = Mean(data);
    const auto deviation = basic_math::minus(data, mean);
    const auto absoluteDeviation = basic_math::abs(gsl::make_span(deviation));
    return Mean(gsl::make_span(absoluteDeviation));
}

/// <summary>
/// Find median absolute deviation of the data. Return 0 when input data is empty.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Median absolute deviation of the elements.</returns>
template <class DataType>
DataType MedianAbsoluteDeviation(gsl::span<DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    gsl::span<const DataType> constSpan(data);
    const auto median = Median(constSpan);
    const auto deviation = basic_math::minus(constSpan, median);
    const auto absoluteDeviation = basic_math::abs(gsl::make_span(deviation));
    return Median(gsl::make_span(absoluteDeviation));
}
}
