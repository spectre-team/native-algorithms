/*
* MaximizationRunnerOpt.h
* Provides a semi-optimized implementation of maximization part of EM algorithm
* used for Gaussian Mixture Modelling.
*
* Knowledge required for understanding the following code has been
* presented in the following document
* https://brilliant.org/wiki/gaussian-mixture-model/#learning-the-model
* which shall serve as a mathematical reference, and to which the
* comments in the code will refer to.
*
* In regard to the following code concerning Maximization procedure,
* please refer to
* Learning the Model
*      Algorithm for Univariate Gaussian Mixture Models
*           Maximization (M) Step
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
#include "Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h"
#include "Spectre.libGaussianMixtureModelling/Matrix.h"

namespace spectre::unsupervised::gmm
{
    /// <summary>
    /// Class serves the purpose of maximization step of Expectation
    /// Maximization algorithm.
    /// </summary>
    class MaximizationRunnerOpt
    {
    public:
        /// <summary>
        /// Constructor initializing the class with data required during
        /// maximization step.
        /// </summary>
        /// <param name="spectrum">Spectrum to base calculations on.</param>
        MaximizationRunnerOpt(SpectrumView spectrum) : m_SummedIntensities(
            std::accumulate(
                spectrum.intensities.begin(), spectrum.intensities.end(), 0.0
                )
            )
        {
        }

        /// <summary>
        /// Updates weights, means and tandard deviations in a single run.
        /// </summary>
        /// <param name="affilationMatrix">Matrix symbolising the probability
        /// of affilation of each sample to a certain gaussian component.
        /// </param>
        /// <param name="spectrum">Spectrum to base calculations on.</param>
        /// <param name="components">Gaussian components to be updated</param>
        void Maximization(Matrix<DataType> &affilationMatrix,
            SpectrumView spectrum, GaussianMixtureModel& components)
        {
            const unsigned numberOfComponents = (unsigned)components.size();
            const unsigned dataSize = (unsigned)spectrum.mzs.size();
            DataView mzs = spectrum.mzs;
            for (unsigned k = 0; k < numberOfComponents; k++)
            { 
                // Using Kahan summation algorithm for improved accuracy
                DataType probabilitySum = 0.0;
                DataType meanNumerator = 0.0;
                DataType probCorrection = 0.0;
                DataType meanCorrection = 0.0;
                for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
                {
                    DataType corrected = affilationMatrix[k][i] - probCorrection;
                    DataType newSum = probabilitySum + corrected;
                    probCorrection = (newSum - probabilitySum) - corrected;
                    probabilitySum = newSum;
                    corrected = affilationMatrix[k][i] * mzs[i] - meanCorrection;
                    newSum = meanNumerator + corrected;
                    meanCorrection = (newSum - meanNumerator) - corrected;
                    meanNumerator = newSum;
                }
                components[k].weight = probabilitySum / m_SummedIntensities;
                components[k].mean = meanNumerator / probabilitySum;
                probCorrection = 0.0;
                DataType stdNumerator = 0.0;
                for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
                {
                    DataType diff = mzs[i] - components[k].mean;
                    DataType corrected = affilationMatrix[k][i] * diff * diff
                        - probCorrection;
                    DataType newSum = stdNumerator + corrected;
                    probCorrection = (newSum - stdNumerator) - corrected;
                    stdNumerator = newSum;
                }
                components[k].deviation = sqrt(stdNumerator/probabilitySum);
            }
        }
    private:
        DataType m_SummedIntensities;
    };
}
