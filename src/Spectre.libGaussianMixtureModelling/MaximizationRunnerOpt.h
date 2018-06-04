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
* Note: throught reading of the document, you may notice that each
* time usage of affiliation matrix occurs it is multiplied by
* corresponding intesity, to increase the probability *intensity*
* times that a certain mz belongs to a given component. This is
* because the spectra we are given are histograms, and therefore
* a single mz value with intensity e.g. 500 indicates that
* there are de facto 500 of entities holding that certain mz value
* that are stored in the dataset.
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
#include "GaussianMixtureModel.h"
#include "Matrix.h"

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
        /// <param name="affilationMatrix">Matrix symbolising the probability
        /// of affilation of each sample to a certain gaussian component.
        /// </param>
        /// <param name="components">Gaussian components to be updated</param>
        MaximizationRunnerOpt(SpectrumView spectrum,
            Matrix<DataType> &affilationMatrix,
            std::vector<GaussianComponent> &components)
            : m_Spectrum(spectrum), m_AffilationMatrix(affilationMatrix)
            , m_Components(components)
            , m_SummedIntensities(std::accumulate(spectrum.intensities.begin(),
                spectrum.intensities.end(), 0.0))
        {
        }

        /// <summary>
        /// Contratry to what name indicates, it updates weights, means and
        /// standard deviations in a single run.
        /// </summary>
        void UpdateWeights()
        {
            const unsigned numberOfComponents = (unsigned)m_Components.size();
            const unsigned dataSize = (unsigned)m_Spectrum.mzs.size();
            DataView mzs = m_Spectrum.mzs;
            for (unsigned k = 0; k < numberOfComponents; k++)
            {
                DataType probabilitySum = 0.0;
                DataType meanNumerator = 0.0;
                for (unsigned i = 0; i < (unsigned)m_Spectrum.mzs.size(); i++)
                {
                    probabilitySum += m_AffilationMatrix[k][i];
                    meanNumerator += m_AffilationMatrix[k][i] * mzs[i];
                }
                m_Components[k].weight = probabilitySum / m_SummedIntensities;
                m_Components[k].mean = meanNumerator / probabilitySum;
                DataType stdNumerator = 0.0;
                for (unsigned i = 0; i < (unsigned)m_Spectrum.mzs.size(); i++)
                {
                    DataType diff = mzs[i] - m_Components[k].mean;
                    stdNumerator += m_AffilationMatrix[k][i] * diff * diff;
                }
                m_Components[k].deviation = sqrt(stdNumerator/probabilitySum);
            }
        }

        /// <summary>
        /// Empty function made to conform with interface.
        /// </summary>
        void UpdateMeans()
        {
        }

        /// <summary>
        /// Empty function made to conform with interface.
        /// </summary>
        void UpdateStdDeviations()
        {
        }

    private:
        SpectrumView m_Spectrum;
        Matrix<DataType> &m_AffilationMatrix;
        std::vector<GaussianComponent> &m_Components;
        DataType m_SummedIntensities;
    };
}
