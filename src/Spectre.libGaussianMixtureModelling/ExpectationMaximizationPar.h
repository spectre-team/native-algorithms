/*
 * ExpectationMaximization.h
 * Provides implementation of Expectation Maximization algorithm 
 * used for Gaussian Mixture Modelling.
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
#include <cmath>
#include "Spectre.libStatistics/Math.h"
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationMaximization.h"
#include "Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h"
#include "Spectre.libGaussianMixtureModelling/Matrix.h"

namespace spectre::unsupervised::gmm
{
static inline void SetMinStdsPar(GaussianMixtureModel& components, DataType minStd)
{
#pragma omp parallel for
    for (int i = 0; i < components.size(); i++)
    {
        components[i].deviation = std::max(components[i].deviation, minStd);
    }
}

static inline DataType CalculateDeltaPar(GaussianMixtureModel& oldModel,
    GaussianMixtureModel& newModel)
{
    using namespace statistics::basic_math;
    DataType weightCorrection = 0.0, heightCorrection = 0.0,
    varianceCorrection = 0.0, sumOfHeightDifferences = 0.0,
    sumOfScaledVarianceDifferences = 0.0, justASum = 0.0;
#pragma omp parallel for reduction(+: justASum, sumOfHeightDifferences, \
                                      sumOfScaledVarianceDifferences)
    for (int i = 0; i < oldModel.size(); i++)
    {
        justASum = accurate_add(justASum, newModel[i].weight, weightCorrection);
        sumOfHeightDifferences = accurate_add(sumOfHeightDifferences,
           std::abs(newModel[i].weight - oldModel[i].weight), heightCorrection);
        sumOfScaledVarianceDifferences =
            accurate_add(sumOfScaledVarianceDifferences,
              std::abs(newModel[i].deviation * newModel[i].deviation -
                       oldModel[i].deviation * oldModel[i].deviation) /
                      (newModel[i].deviation * newModel[i].deviation),
                varianceCorrection);
    }
    sumOfScaledVarianceDifferences /= (DataType)oldModel.size();
    return sumOfHeightDifferences + sumOfScaledVarianceDifferences;
}

/// <summary>
/// Class serves as the container for all data required by
/// A. P. Dempster's Expectation Maximization algorithm used
/// for the purpose of Guassian Mixture Modelling of the
/// spectral data.
/// </summary>
/// <param name="ExpectationRunner">
/// Class performing expectation step of the em algorithm.
/// </param>
/// <param name="MaximizationRunner">
/// Class performing maximization step of the em algorithm.
/// </param>
template <typename ExpectationRunner, typename MaximizationRunner>
class ExpectationMaximizationPar
{
public:
    /// <summary>
    /// Constructor initializing the class with all algorithm necessary data.
    /// </summary>
    /// <param name="spectrum">Spectrum to be approximated.</param>
    /// <param name="maxNumOfComponent">Number of Gaussian components
    /// that build up the approximation.</param>
    ExpectationMaximizationPar(SpectrumView spectrum,
                            unsigned maxNumOfComponents)
          : m_AffilationMatrix(maxNumOfComponents,(unsigned)spectrum.mzs.size())
          , m_Expectation(spectrum)
          , m_Maximization(spectrum)
    {
    }

    /// <summary>
    /// Performs a full algorithm run. Terminates when changes in weights and
    /// heights are below threshold defined by epsilon.
    /// </summary>
    /// <param name="spectrum">Data to approximate.</param>
    /// <param name="components">Base components to improve.</param>
    /// <param name="minStd">
    /// Minimal standard deviation a component can have.
    /// </param>
    /// <param name="epsilon">
    /// Threshold determining max difference in parameters between consecutive
    /// iterations.
    /// </param>
    /// <returns>
    /// Gaussian Mixture Model containing all the components with their
    /// appropriate parameters.
    /// </returns>
    GaussianMixtureModel EstimateGmm(SpectrumView spectrum,
        GaussianMixtureModel& components, DataType minStd, DataType epsilon)
    {
        SetMinStds(components, minStd);
        GaussianMixtureModel oldModel(components.size());

        DataType delta; // How much did the last iteration change the results.
        do
        {
            FilterLowHeightComponents(components);
            oldModel.assign(components.begin(), components.end());
            m_Expectation.Expectation(m_AffilationMatrix, spectrum, components);
            m_Maximization.Maximization(m_AffilationMatrix, spectrum,
                components);
            SetMinStdsPar(components, minStd);
            delta = CalculateDeltaPar(oldModel, components);
        } while (delta > epsilon);

        Sort(components);
        return components;
    }

private:
    Matrix<DataType> m_AffilationMatrix;
    ExpectationRunner m_Expectation;
    MaximizationRunner m_Maximization;
};
}
