/*
 * ExpectationMaximization.h
 * Provides implementation of Expectation Maximization algorithm 
 * used for Gaussian Mixture Modelling.
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
#include <random>
#include "Spectre.libException/NullPointerException.h"
#include "DataTypes.h"
#include "GaussianMixtureModel.h"
#include "Matrix.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Class serves as the container for all data required by
/// A. P. Dempster's Expectation Maximization algorithm used
/// for the purpose of Guassian Mixture Modelling of the
/// spectral data.
/// </summary>
/// <param name="InitializationRunner">Class performing Initialization step of the em algorithm.</param>
/// <param name="ExpectationRunner">Class performing expectation step of the em algorithm.</param>
/// <param name="MaximizationRunner">Class performing maximization step of the em algorithm.</param>
/// <param name="LogLikelihoodCalculator">Class performing log likelihood of resulting calculation.</param>
template <typename InitializationRunner, typename ExpectationRunner, typename MaximizationRunner, typename LogLikelihoodCalculator>
class ExpectationMaximization
{
public:
    /// <summary>
    /// Constructor initializing the class with all algorithm necessary data.
    /// </summary>
    /// <param name="spectrum">Spectrum to be approximated</param>
    /// <param name="rngEngine">Mersenne-Twister engine to be used during initialization step.</param>
    /// <param name="numberOfComponents">Number of Gaussian components that build up the approximation.</param>
    /// <exception cref="NullPointerException">Thrown when either of mzArray or intensities pointers are null</exception>
    ExpectationMaximization(SpectrumView spectrum, RandomNumberGenerator &rngEngine, const unsigned numberOfComponents = 2)
        : m_Spectrum(std::move(spectrum)), m_Components(numberOfComponents)
          , m_AffilationMatrix((unsigned)spectrum.mzs.size(), numberOfComponents)
          , m_Initialization(spectrum.mzs, m_Components, rngEngine)
          , m_Expectation(spectrum.mzs, m_AffilationMatrix, m_Components)
          , m_Maximization(spectrum, m_AffilationMatrix, m_Components)
          , m_LogLikelihoodCalculator(spectrum, m_Components)
    {
    }

    /// <summary>
    /// Performs a full algorithm run. Terminates when change in log likelihood
    /// is sufficiently small (lower than  0.00000001).
    /// </summary>
    /// <returns>
    /// Gaussian Mixture Model containing all the components with their appropriate
    /// parameters.
    /// </returns>
    GaussianMixtureModel EstimateGmm()
    {
        constexpr DataType MIN_LIKELIHOOD_CHANGE = 0.00000001;
        Initialization();

        DataType oldLikelihood; // used as iterations terminator
        DataType newLikelihood = m_LogLikelihoodCalculator.CalculateLikelihood();
        do
        {
            Expectation();
            oldLikelihood = newLikelihood;
            Maximization();
            newLikelihood = m_LogLikelihoodCalculator.CalculateLikelihood();
        }
        while (abs(oldLikelihood - newLikelihood) > MIN_LIKELIHOOD_CHANGE);

        return GaussianMixtureModel(
            m_Spectrum.mzs,
            m_Spectrum.intensities,
            std::move(m_Components)
        );
    }

private:
    void Initialization()
    {
        m_Initialization.AssignRandomMeans();
        m_Initialization.AssignVariances();
        m_Initialization.AssignWeights();
    }

    void Expectation()
    {
        m_Expectation.Expectation();
    }

    void Maximization()
    {
        m_Maximization.UpdateWeights();
        m_Maximization.UpdateMeans();
        m_Maximization.UpdateStdDeviations();
    }

    Matrix<DataType> m_AffilationMatrix;
    SpectrumView m_Spectrum;
    std::vector<GaussianComponent> m_Components;

    InitializationRunner m_Initialization;
    ExpectationRunner m_Expectation;
    MaximizationRunner m_Maximization;
    LogLikelihoodCalculator m_LogLikelihoodCalculator;
};
}
