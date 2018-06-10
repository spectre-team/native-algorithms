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
#include <cmath>
#include <random>
#include "Spectre.libException/NullPointerException.h"
#include "Spectre.libStatistics/Math.h"
#include "DataTypes.h"
#include "GaussianMixtureModel.h"
#include "Matrix.h"

namespace spectre::unsupervised::gmm
{
static inline void SetMinStds(GaussianMixtureModel& components, DataType minStd)
{
    for (Index i = 0; i < components.size(); i++)
    {
        components[i].deviation = std::max(components[i].deviation, minStd);
    }
}

static inline void FilterLowHeightComponents(GaussianMixtureModel& components)
{
    constexpr DataType MIN_WEIGHT = 1.0e-3; // Height, weight are interchangable

    GaussianMixtureModel::iterator pastEndIter =
        std::remove_if(components.begin(), components.end(),
            [MIN_WEIGHT](GaussianComponent& component)
            { return component.weight < MIN_WEIGHT; });
    size_t newSize = pastEndIter - components.begin();
    components.resize(newSize);
}

static inline DataType CalculateDelta(GaussianMixtureModel& oldModel,
    GaussianMixtureModel& newModel)
{
    DataType sumOfHeightDifferences = 0.0;
    DataType sumOfScaledVarianceDifferences = 0.0;
    DataType justASum = 0.0;
    for (Index i = 0; i < oldModel.size(); i++)
    {
        justASum += newModel[i].weight;
        sumOfHeightDifferences += abs(newModel[i].weight - oldModel[i].weight);
        sumOfScaledVarianceDifferences +=
            abs(newModel[i].deviation * newModel[i].deviation -
                oldModel[i].deviation * oldModel[i].deviation) /
               (newModel[i].deviation * newModel[i].deviation);
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
class ExpectationMaximization
{
public:
    /// <summary>
    /// Constructor initializing the class with all algorithm necessary data.
    /// </summary>
    /// <param name="spectrum">Spectrum to be approximated.</param>
    /// <param name="maxNumOfComponent">Number of Gaussian components
    /// that build up the approximation.</param>
    ExpectationMaximization(SpectrumView spectrum,
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
            delta = CalculateDelta(oldModel, components);
        } while (delta > epsilon);

        return components;
    }

private:
    Matrix<DataType> m_AffilationMatrix;
    ExpectationRunner m_Expectation;
    MaximizationRunner m_Maximization;
};
}
