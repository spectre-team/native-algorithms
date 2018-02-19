﻿/*
* SVMFitnessFunction.h
* Trains data in svm and returns score of a Individual.
*
Copyright 2017 Spectre Team

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
#include "Spectre.libClassifier/ConfusionMatrix.h"
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libGenetic/FitnessFunction.h"
#include "RaportGenerator.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// Fitness function based on openCV SVM algorithm.
/// </summary>
class SVMFitnessFunction : public spectre::algorithm::genetic::FitnessFunction
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="SVMFitnessFunction"/> class.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="raportGenerator">The raport generator.</param>
    /// <param name="independentValidation">The independent validation.</param>
    /// <param name="svmIterations">The number of SVM iterations.</param>
    /// <param name="svmTolerance">The SVM tolerance.</param>
    SVMFitnessFunction(spectre::supervised::SplittedOpenCvDataset&& data,
                       scenario::gasvm::RaportGenerator& raportGenerator,
                       const spectre::supervised::OpenCvDataset* independentValidation = nullptr,
                       unsigned int svmIterations=100u,
                       double svmTolerance=1e-6);
    /// <summary>
    /// Computes fitness.
    /// </summary>
    /// <param name="other">The other population.</param>
    /// <returns>Self.</returns>
    spectre::algorithm::genetic::ScoreType operator()(const spectre::algorithm::genetic::Individual &individual) override;
    virtual ~SVMFitnessFunction() = default;
private:
    /// <summary>
    /// The dataset.
    /// </summary>
    spectre::supervised::SplittedOpenCvDataset m_Dataset;
    /// <summary>
    /// The independent validation.
    /// </summary>
    const spectre::supervised::OpenCvDataset* m_IndependentValidation;
    /// <summary>
    /// The report generator.
    /// </summary>
    scenario::gasvm::RaportGenerator& m_RaportGenerator;
    /// <summary>
    /// The number of SVM iterations.
    /// </summary>
    const uint m_SvmIterations;
    /// <summary>
    /// The SVM tolerance.
    /// </summary>
    const double m_SvmTolerance;

    /// <summary>
    /// Gets individual confusion matrix.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="individual">The individual.</param>
    /// <returns>The confusion matrix.</returns>
    spectre::supervised::ConfusionMatrix getResultMatrix(const spectre::supervised::OpenCvDataset& data, const spectre::algorithm::genetic::Individual& individual) const;
};
}
