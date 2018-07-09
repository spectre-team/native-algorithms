/*
* Math.h
* Basic mathematical operations on vectors.
*
Copyright 2017 Grzegorz Mrukwa

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
#include <cmath>
#include <vector>
#include <gsl/span>
#include "Spectre.libFunctional/Transform.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"

namespace spectre::statistics::basic_math
{
// @gmrukwa: vector vs vector

/// <summary>
/// Add two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are sums of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> plus(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left + right; });
}

/// <summary>
/// Subtract two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are difference of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> minus(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left - right; });
}

/// <summary>
/// Multiply two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are multiplications of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> multiplyBy(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left * right; });
}

/// <summary>
/// Divide two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are divisions of corresponding elements in first by the second.</returns>
template< class DataType >
constexpr std::vector<DataType> divideBy(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left / right; });
}

/// <summary>
/// Calculates modulo of two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are modulo of corresponding elements in first by the second.</returns>
template< class DataType >
constexpr std::vector<DataType> modulo(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left % right; });
}

/// <summary>
/// AND bitwise two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are bitwise ANDs of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> bitwiseAnd(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left & right; });
}

/// <summary>
/// AND logically two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are logical ANDs of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> logicalAnd(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left && right; });
}

/// <summary>
/// OR bitwise two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are bitwise ORs of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> bitwiseOr(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left | right; });
}

/// <summary>
/// OR logically two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are logical ORs of corresponding elements in source vectors.</returns>
template< class DataType >
constexpr std::vector<DataType> logicalOr(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left || right; });
}

/// <summary>
/// Select max of two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are maxes of corresponding elements in source vectors.</returns>
template<class DataType>
constexpr std::vector<DataType> max(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return std::max(left, right); });
}

/// <summary>
/// Select min of two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are mins of corresponding elements in source vectors.</returns>
template<class DataType>
constexpr std::vector<DataType> min(gsl::span<const DataType> first, gsl::span<const DataType> second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return std::min(left, right); });
}

/// <summary>
/// Checks equality of two vectors elementwise.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector, elements of which are true, if corresponding elements in source vectors were equal.</returns>
template<class DataType>
std::vector<bool> equals(gsl::span<const DataType> first, gsl::span<const DataType> second, DataType* /*dummy*/ = static_cast<DataType *>(nullptr))
{
    return spectre::core::functional::transform(first, second, [](DataType left, DataType right) { return left == right; }, static_cast<bool*>(nullptr));
}

// @gmrukwa: vector vs scalar

/// <summary>
/// Add a constant to a vector.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector first which elements are incremented by value of second.</returns>
template< class DataType >
constexpr std::vector<DataType> plus(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left + second; });
}

/// <summary>
/// Subtract a constant from a vector.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector first which elements are decremented by value of second.</returns>
template< class DataType >
constexpr std::vector<DataType> minus(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left - second; });
}

/// <summary>
/// Multiply a vector by a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector first which elements are multiplied by value of second.</returns>
template< class DataType >
constexpr std::vector<DataType> multiplyBy(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left * second; });
}

/// <summary>
/// Divide a vector by a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector first which elements are divided by value of second.</returns>
template< class DataType >
constexpr std::vector<DataType> divideBy(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left / second; });
}

/// <summary>
/// Find modulo of each element of vector when divided by a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector first which elements were subject to modulo by value of second</returns>
template< class DataType >
constexpr std::vector<DataType> modulo(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left % second; });
}

/// <summary>
/// AND bitwise vector content with a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which elements were subject to bitwise AND with values of second</returns>
template< class DataType >
constexpr std::vector<DataType> bitwiseAnd(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left & second; });
}

/// <summary>
/// AND logically vector content with a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which elements were subject to logical AND with values of second</returns>
template< class DataType >
constexpr std::vector<DataType> logicalAnd(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left && second; });
}

/// <summary>
/// OR bitwise vector content with a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which elements were subject to bitwise OR with values of second</returns>
template< class DataType >
constexpr std::vector<DataType> bitwiseOr(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left | second; });
}

/// <summary>
/// OR logically vector content with a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which elements were subject to logical OR with values of second</returns>
template< class DataType >
constexpr std::vector<DataType> logicalOr(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return left || second; });
}

/// <summary>
/// Select max of each vector value - constant pair.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which consist of either original vector content or the constant</returns>
template<class DataType>
constexpr std::vector<DataType> max(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return std::max(left, second); });
}

/// <summary>
/// Select min of each vector value - constant pair.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which consist of either original vector content or the constant</returns>
template<class DataType>
constexpr std::vector<DataType> min(gsl::span<const DataType> first, DataType second)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(first, [second](DataType left) { return std::min(left, second); });
}

/// <summary>
/// Check equality of vector content with respect to a constant.
/// </summary>
/// <param name="first">The first.</param>
/// <param name="second">The second.</param>
/// <returns>Vector which elements are true where elements of first were second.</returns>
template<class DataType>
std::vector<bool> equals(gsl::span<const DataType> first, DataType second)
{
    return spectre::core::functional::transform(first, [second](DataType left) { return left == second; }, static_cast<bool*>(nullptr));
}

// @gmrukwa: unary

/// <summary>
/// Find absolute value of a vector content.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Vector consisting of absolute values of elements of first</returns>
template <class DataType>
constexpr std::vector<DataType> abs(gsl::span<const DataType> data)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    return spectre::core::functional::transform(data, [](DataType entry) { return std::abs(entry); });
}

/// <summary>
/// transform to opposite values of vector of bool
/// </summary>
/// <param name="data">The data.</param>
/// <returns>Vector of bools consisting of opposite values of elements of data</returns>
inline std::vector<bool> negate(std::vector<bool> data)
{
    return spectre::core::functional::transform(data, [](bool entry) { return !entry; });
}

/// <summary>
/// Builds vector of the specified size.
/// </summary>
/// <param name="size">The size.</param>
/// <param name="predicate">The generator.</param>
/// <returns>Vector of generated elements.</returns>
template <class DataType, class Generator>
std::vector<DataType> build(size_t size, Generator generator)
{
    std::vector<DataType> result;
    result.reserve(size);
    for(auto i=0u; i<size; ++i)
    {
        result.push_back(generator());
    }
    return result;
}

/// <summary>
/// Calculates differences between adjacent elements of given container.
/// The function is based upon Matlab function diff():
/// https://www.mathworks.com/help/matlab/ref/diff.html
/// but it can handle only one dimensional data.
/// </summary>
/// <param name="data">The container.</param>
/// <param name="order">Order of difference operation (how many times the difference is executed).</param>
/// <returns>Vector of calculated differences, of size: length_of_data - order.</returns>
template <class DataType>
std::vector<typename std::remove_const<DataType>::type> differentiate_unsafe(gsl::span<DataType> data, uint16_t order = 1)
{
    std::vector<typename std::remove_const<DataType>::type> result(data.begin(), data.end());
    for (auto i = 0u; i < order; ++i)
    {
        for (auto j = 0u; j < result.size() - 1; ++j)
            result[j] = result[j + 1] - result[j];
        result.resize(result.size() - 1);
    }
    return result;
}

/// <summary>
/// Safe version of <see cref="differentiate_unsafe"/>.
/// Validates the input before performing algorithm.
/// </summary>
/// <param name="data">The container.</param>
/// <param name="order">Order of difference operation (how many times the difference is executed).</param>
/// <returns>Vector of calculated differences, of size: length_of_data - order.</returns>
template <class DataType>
std::vector<typename std::remove_const<DataType>::type> differentiate(gsl::span<DataType> data, uint16_t order = 1)
{
    size_t length = data.length();
    if (length < 2)
        throw spectre::core::exception::ArgumentOutOfRangeException<size_t>("data.length()", 2, SIZE_MAX, length);
    if (order >= length)
        throw spectre::core::exception::ArgumentOutOfRangeException<size_t>("order", 0, length, size_t(order));
    return differentiate_unsafe(data, order);
}

/// <summary>
/// Peroforms an accurate summation according to Kahan summation scheme.
///
/// https://en.wikipedia.org/wiki/Kahan_summation_algorithm#The_algorithm
///
/// The function performs operations corresponding to the inside of the
/// loop. The variable 'c' in the example above corresponds to the 'corrrection'
/// parameter of the function. 
/// </summary>
/// <param name="sum">Current sum to be added to.</param>
/// <param name="value">Number to be added.</param>
/// <param name="correction">Variable assuring the correct precision.</param>
/// <returns>.</returns>
template <class DataType>
static DataType accurate_add(DataType sum, DataType value, DataType& correction)
{
    static_assert(std::is_arithmetic_v<DataType>, "DataType: expected arithmetic.");
    value -= correction;
    DataType newSum = sum + value;
    correction = (newSum - sum) - value;
    return newSum;
}
}
