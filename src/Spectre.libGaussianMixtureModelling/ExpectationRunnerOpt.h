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
#include "DataTypes.h"
#include "GaussianDistribution.h"
#include "GaussianMixtureModel.h"
#include "Matrix.h"

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
        /// <param name="spectrum">Spectrum to operate on.</param>
        /// <param name="affilationMatrix">Matrix symbolising the probability
        /// of affilation of each sample to a certain gaussian component.
        /// </param>
        /// <param name="components">Gaussian components.</param>
        ExpectationRunnerOpt(SpectrumView spectrum,
            Matrix<DataType> &affilationMatrix,
            std::vector<GaussianComponent> &components)
            : m_Spectrum(spectrum), m_Components(components)
            , m_AffilationMatrix(affilationMatrix)
            , m_Sums(spectrum.mzs.size())
        {
        }

        /// <summary>
        /// Fills affilation (gamma) matrix with probabilities of affilation
        /// of each sample to a certain gaussian component.
        /// </summary>
        void Expectation()
        {
            DataView mzs = m_Spectrum.mzs;
            DataView intensities = m_Spectrum.intensities;
            std::fill(m_Sums.begin(), m_Sums.end(), 0.0);
            const unsigned numberOfComponents = (unsigned)m_Components.size();
            for (unsigned k = 0; k < numberOfComponents - 1; k++)
            {
                for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
                {
                    DataType probability = m_Components[k].weight *
                        Gaussian(mzs[i], m_Components[k].mean,
                            m_Components[k].deviation);
                    m_AffilationMatrix[k][i] = probability;
                    m_Sums[i] += probability;
                }
            }
            const unsigned lastK = numberOfComponents - 1;
            for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
            {
                DataType probability = m_Components[lastK].weight *
                    Gaussian(mzs[i], m_Components[lastK].mean,
                        m_Components[lastK].deviation);
                m_AffilationMatrix[lastK][i] = probability;
                m_Sums[i] = 1.0 / (m_Sums[i] + probability);
            }

            for (unsigned k = 0; k < numberOfComponents; k++)
            {
                for (unsigned i = 0; i < (unsigned)mzs.size(); i++)
                {
                    m_AffilationMatrix[k][i] *= m_Sums[i] * intensities[i];
                }
            }
        }

    private:
        SpectrumView m_Spectrum;
        Matrix<DataType> &m_AffilationMatrix;
        std::vector<GaussianComponent> &m_Components;
        Data m_Sums; // holds intermediate sums of probabilites for each point.
    };
}
