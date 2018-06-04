/*
* GaussianDistribution.h
* Provides implementation of Gaussian Distribution function (bell curve).
*
Copyright 2017 Michal Gallus

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
#define _USE_MATH_DEFINES // used for M_PI
#include <math.h>
#include "DataTypes.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Computes value of Guassian Function, also known as Normal distribution
/// based on given mean, standard deviation for a single observation.
/// </summary>
/// <param name="x">Argument to compute value for.</param>
/// <param name="mean">Mean of the distribution.</param>
/// <param name="std">Standard deviation of the distribution.</param>
inline DataType Gaussian(DataType x, DataType mean, DataType std)
{
    static const DataType inverseOfSqrtTwoPi = 1.0 / sqrt(2.0 * M_PI);
    DataType twoExpSqrt = (x - mean) / std;
    return inverseOfSqrtTwoPi * std::exp(-0.5 * twoExpSqrt * twoExpSqrt) / std;
}

/// <summary>
/// Serves as a representation of Normal Distribution with fixed parameters.
/// </summary>
class GaussianDistribution
{
public:
    /// <summary>
    /// Constructs a distrubution based on given mean and standard deviation.
    /// </summary>
    /// <param name="mean">Mean of the distribution.</param>
    /// <param name="std">Standard deviation of the distribution.</param>
    GaussianDistribution(DataType mean, DataType std) : m_Mean(mean),
        m_Exponent(-0.5 / (std * std))
    {
        static const DataType inverseOfSqrtTwoPi = 1.0 / sqrt(2.0 * M_PI);
        m_Scalar = inverseOfSqrtTwoPi / std;
    }

    /// <summary>
    /// Computes the probability based on given argument and current mean
    /// and standard deviation.
    /// </summary>
    /// <param name="x">Argument to compute value for.</param>
    DataType Compute(DataType x)
    {
        DataType diff = (x - m_Mean);
        return m_Scalar * std::exp(m_Exponent * diff * diff);
    }

private:
    DataType m_Mean;
    DataType m_Scalar;
    DataType m_Exponent;
};
}
