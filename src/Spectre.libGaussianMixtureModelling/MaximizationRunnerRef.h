/*
 * MaximizationRunnerRef.h
 * Provides reference implementation of maximization part of EM algorithm
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
#include <vector>
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h"
#include "Spectre.libGaussianMixtureModelling/Matrix.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Class serves the purpose of maximization step of Expectation Maximization
/// algorithm. Serves as a reference to learn from and is purpously not
/// optimized to avoid obfuscation of the code.
/// </summary>
class MaximizationRunnerRef
{
public:
    /// <summary>
    /// Constructor initializing the class with data required during
    /// maximization step.
    /// </summary>
    /// <param name="spectrum">Spectrum to base calculations on.</param>
    MaximizationRunnerRef(SpectrumView)
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
        UpdateWeights(affilationMatrix, spectrum, components);
        UpdateMeans(affilationMatrix, spectrum, components);
        UpdateStdDeviations(affilationMatrix, spectrum, components);
    }

private:
    /// <summary>
    /// Updates weights in gaussian components, based on affilation matrix.
    /// </summary>
    /// <param name="affilationMatrix">Matrix symbolising the probability
    /// of affilation of each sample to a certain gaussian component.
    /// </param>
    /// <param name="spectrum">Spectrum to base calculations on.</param>
    /// <param name="components">Gaussian components to be updated</param>
    void UpdateWeights(Matrix<DataType> &affilationMatrix,
        SpectrumView spectrum, GaussianMixtureModel& components)
    {
        // This part conducts the instruction:
        // "Using the gamma calculated in the Expectation step, 
        // calculate in the following order, for every k: (phi part)"
        // from the document
        DataType totalDataSize = 0.0;
        for (unsigned i = 0; i < (unsigned)spectrum.mzs.size(); i++)
        {
            totalDataSize += spectrum.intensities[i];
        }

        const unsigned numberOfComponents = (unsigned)components.size();
        for (unsigned k = 0; k < numberOfComponents; k++)
        {
            DataType weight = 0.0;
            for (unsigned i = 0; i < (unsigned)spectrum.mzs.size(); i++)
            {
                weight += affilationMatrix[k][i] * spectrum.intensities[i];
            }
            components[k].weight = weight / totalDataSize;
        }
    }

    /// <summary>
    /// Updates means in gaussian components, based on affilation matrix.
    /// </summary>
    /// <param name="affilationMatrix">Matrix symbolising the probability
    /// of affilation of each sample to a certain gaussian component.
    /// </param>
    /// <param name="spectrum">Spectrum to base calculations on.</param>
    /// <param name="components">Gaussian components to be updated</param>
    void UpdateMeans(Matrix<DataType> &affilationMatrix,
        SpectrumView spectrum, GaussianMixtureModel& components)
    {
        // This part conducts the instruction:
        // "Using the gamma calculated in the Expectation step, 
        // calculate in the following order, for every k: (mu part)"
        // from the document
        const unsigned numberOfComponents = (unsigned)components.size();
        for (unsigned k = 0; k < numberOfComponents; k++)
        {
            DataType denominator = 0.0;
            DataType numerator = 0.0;
            for (unsigned i = 0; i < (unsigned)spectrum.mzs.size(); i++)
            {
                denominator += affilationMatrix[k][i] * spectrum.intensities[i];
                numerator += affilationMatrix[k][i] * spectrum.mzs[i] *
                    spectrum.intensities[i];
            }
            components[k].mean = numerator / denominator;
        }
    }

    /// <summary>
    /// Updates standard deviations in gaussian components, based on affilation
    /// (gamma) matrix.
    /// </summary>
    /// <param name="affilationMatrix">Matrix symbolising the probability
    /// of affilation of each sample to a certain gaussian component.
    /// </param>
    /// <param name="spectrum">Spectrum to base calculations on.</param>
    /// <param name="components">Gaussian components to be updated</param>
    void UpdateStdDeviations(Matrix<DataType> &affilationMatrix,
        SpectrumView spectrum, GaussianMixtureModel& components)
    {
        // This part conducts the instruction:
        // "Using the gamma calculated in the Expectation step, 
        // calculate in the following order, for every k: (sigma part)"
        // from the document.
        // Please note, that equation presented in the document 
        // is wrong in that it states that the right hand side of the
        // equation is equal to sigma, while in a fact, it should be 
        // equated to sigma^2. Hence the sqrt() is used during the 
        // assignment of the standard deviation.
        const unsigned numberOfComponents = (unsigned)components.size();
        for (unsigned k = 0; k < numberOfComponents; k++)
        {
            DataType denominator = 0.0;
            DataType numerator = 0.0;
            for (unsigned i = 0; i < (unsigned)spectrum.mzs.size(); i++)
            {
                denominator += affilationMatrix[k][i] * spectrum.intensities[i];
                numerator += affilationMatrix[k][i] *
                    pow(spectrum.mzs[i] - components[k].mean, 2) *
                    spectrum.intensities[i];
            }
            components[k].deviation = sqrt(numerator / denominator);
        }
    }
};
}
