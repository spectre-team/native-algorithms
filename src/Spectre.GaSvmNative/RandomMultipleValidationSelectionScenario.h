/*
* RandomMultipleValidationSelectionScenario.h
* Executes genetic algorithm and normal svm and returns results.
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
#include <vector>
#include "Spectre.libGenetic/DataTypes.h"
#include <opencv2/core/cvdef.h>
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libGenetic/GeneticAlgorithmFactory.h"

namespace Spectre::GaSvmNative
{
class RandomMultipleValidationSelectionScenario
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="GeneticTrainingSetSelectionScenario"/> class.
    /// </summary>
    /// <param name="trainingSetSplitRate">The training set split rate.</param>
    /// <param name="mutationRate">The mutation rate.</param>
    /// <param name="bitSwapRate">The bit swap rate.</param>
    /// <param name="preservationRate">The preservation rate.</param>
    /// <param name="generationsNumber">The generation number.</param>
    /// <param name="populationSizes">The population sizes.</param>
    /// <param name="initialFillups">The initial fillups.</param>
    /// <param name="reportFilename">The raport filename.</param>
    /// <param name="numberOfRestarts">The number of restarts.</param>
    /// <param name="numberOfCores">The number of cores.</param>
    /// <param name="seed">The seed.</param>
    /// <param name="minimalFillup">The minimal fillup.</param>
    /// <param name="maximalFillup">The maximal fillup.</param>
    /// <param name="svmIterations">The svm iterations.</param>
    /// <param name="svmTolerance">The svm tolerance.</param>
    RandomMultipleValidationSelectionScenario(double trainingSetSplitRate,
        double downsampleTrainingRate,
        size_t maximumSubsetSize,
        double mutationRate,
        double bitSwapRate,
        double preservationRate,
        unsigned int generationsNumber,
        const unsigned int populationSize,
        const unsigned int initialFillup,
        const std::string& reportFilename,
        unsigned int numberOfRestarts,
        unsigned int numberOfCores = 1u,
        Spectre::libGenetic::Seed seed = 0,
        size_t minimalFillup = 1ul,
        size_t maximalFillup = std::numeric_limits<size_t>::max(),
        uint svmIterations = 100u,
        double svmTolerance = 1e-6);
    /// <summary>
    /// Executes the scenario.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="independentValidation">The independent validation.</param>
    /// <returns>void</returns>
    void execute(libClassifier::OpenCvDataset &data,
                 const libClassifier::OpenCvDataset *independentValidation = nullptr) const;
private:
    /// <summary>
    /// The population sizes.
    /// </summary>
    const unsigned int m_PopulationSize;
    /// <summary>
    /// The initial individual fillups.
    /// </summary>
    const unsigned int m_InitialIndividualFillup;
    /// <summary>
    /// The training dataset size rate.
    /// </summary>
    const double m_TrainingDatasetSizeRate;
    /// <summary>
    /// The downsample training rate.
    /// </summary>
    const double m_DownsampleTrainingRate;
    /// <summary>
    /// The maximum subset size.
    /// </summary>
    const size_t m_MaximumSubsetSize;
    /// <summary>
    /// The filename.
    /// </summary>
    const std::string m_Filename;
    /// <summary>
    /// The restart number.
    /// </summary>
    const unsigned int m_RestartsNumber;
    /// <summary>
    /// The number of cores.
    /// </summary>
    const unsigned int m_NumberOfCores;
    /// <summary>
    /// The seed.
    /// </summary>
    const libGenetic::Seed m_Seed;
    /// <summary>
    /// The genetic algorithm factory.
    /// </summary>
    const libGenetic::GeneticAlgorithmFactory m_GaFactory;
    /// <summary>
    /// The SVM iterations.
    /// </summary>
    const uint m_SvmIterations;
    /// <summary>
    /// The SVM tolerance.
    /// </summary>
    const double m_SvmTolerance;
};
}
