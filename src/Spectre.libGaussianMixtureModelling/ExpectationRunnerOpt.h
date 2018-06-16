/*
 * ExpectationRunnerOpt.h
 * Provides a semi-optimized implementation of expectation part 
 * of EM algorithm used for Gaussian Mixture Modelling.
 *
 * Knowledge required for understanding the following code has been
 * presented in the following document
 * https://brilliant.org/wiki/gaussian-mixture-model/#learning-the-model
 * which shall serve as a mathematical reference, and to which the
 * comments in the code will refer to.
 *
 * In regard to the following code concerning Expectation procedure,
 * please refer to
 * Learning the Model
 *      Algorithm for Univariate Gaussian Mixture Models
 *           Expectation (E) Step
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
#pragma once
#include <vector>
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/GaussianDistribution.h"
#include "Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h"
#include "Spectre.libGaussianMixtureModelling/Matrix.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Class serves the purpose of expectation step of Expectation
/// Maximization algorithm. It is partially optimized, but still uses
/// just a single thread to compute.
/// </summary>
class ExpectationRunnerOpt
{
public:
    /// <summary>
    /// Constructor initializing the class with data required during
    /// expectation step.
    /// </summary>
    /// <param name="spectrum">Spectrum to initializie sum cache.</param>
    ExpectationRunnerOpt(SpectrumView spectrum): m_Sums(spectrum.mzs.size())
    {
    }

    /// <summary>
    /// Fills affilation (gamma) matrix with probabilities of affilation
    /// of each sample to a certain gaussian component.
    /// </summary>
    /// <param name="affilationMatrix">Matrix symbolising the probability
    /// of affilation of each sample to a certain gaussian component that
    /// is to be updated.</param>
    /// <param name="spectrum">Spectrum to operate on.</param>
    /// <param name="components">Gaussian components</param>
    void Expectation(Matrix<DataType> &affilationMatrix,
        SpectrumView spectrum, GaussianMixtureModel& components)
    {
        DataView mzs = spectrum.mzs;
        DataView intensities = spectrum.intensities;
        std::fill(m_Sums.begin(), m_Sums.end(), 0.0);
        const unsigned numberOfComponents = (unsigned)components.size();
        for (unsigned k = 0; k < numberOfComponents; k++)
        {
            // Using Kahan summation algorithm for improved accuracy
            DataType correction = 0.0;
            for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
            {
                DataType probability = components[k].weight *
                    Gaussian(mzs[i], components[k].mean,
                        components[k].deviation);
                affilationMatrix[k][i] = probability;
                probability -= correction;
                DataType sum = m_Sums[i];
                DataType newSum = sum + probability;
                correction = (newSum - sum) - probability;
                m_Sums[i] = newSum;
            }
        }
        DataType minSum = *std::min_element(m_Sums.begin(), m_Sums.end());
        minSum =
            minSum > 0.0 ? minSum : std::numeric_limits<DataType>::min();
        for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
        {
            m_Sums[i] = 1.0 / std::max(m_Sums[i], minSum);
        }

        for (unsigned k = 0; k < numberOfComponents; k++)
        {
            for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
            {
                affilationMatrix[k][i] *= m_Sums[i] * intensities[i];
            }
        }
    }

private:
    Data m_Sums; // holds intermediate sums of probabilites for each point.
};
}
