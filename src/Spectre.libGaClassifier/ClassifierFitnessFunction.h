/*
* ClassifierFitnessFunction.h
* It is fitness function for any classifier for GaClassifier.
*
Copyright 2018 Spectre Team

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
#include "Spectre.libGenetic/FitnessFunction.h"
#include "Spectre.libClassifier/IClassifier.h"
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libClassifier/ConfusionMatrix.h"

namespace spectre::supervised
{
/// <summary>
/// It is fitness function for any classifier for GaClassifier.
/// </summary>
class ClassifierFitnessFunction : public spectre::algorithm::genetic::FitnessFunction
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="GaFitnessFunction"/> class.
    /// </summary>
    /// <param name="classifier">The classifier.</param>
    /// <param name="data">The splitted data.</param>
    ClassifierFitnessFunction(const IClassifier& classifier, SplittedOpenCvDataset& data);
    /// <summary>
    /// operator.
    /// </summary>
    /// <param name="individual">The individual.</param>
    /// <returns>score.</returns>
    algorithm::genetic::ScoreType operator()(const algorithm::genetic::Individual &individual) override;
    /// <summary>
    /// Gets individual confusion matrix.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <returns>The confusion matrix.</returns>
    ConfusionMatrix getResultMatrix(const OpenCvDataset& data) const;
    virtual ~ClassifierFitnessFunction() = default;
private:
    /// <summary>
    /// The classifier.
    /// </summary>
    const IClassifier& m_Classifier;
    /// <summary>
    /// The splitted data.
    /// </summary>
    const SplittedOpenCvDataset& m_Data;
};
}
