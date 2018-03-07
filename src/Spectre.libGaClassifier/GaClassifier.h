/*
* GaClassifier.h
* Class that searches best classifier via Genentic Algorithm.
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
#include <vector>
#include "Spectre.libGenetic/DataTypes.h"
#include "Spectre.libClassifier/IClassifier.h"
#include "Spectre.libGenetic/GeneticAlgorithmFactory.h"
#include "Spectre.libClassifier/OpenCvDataset.h"

namespace spectre::supervised
{
/// <summary>
/// Class that searches best classifier via Genentic Algorithm.
/// </summary>
class GaClassifier : public IClassifier
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="GeneticTrainingSetSelectionScenario"/> class.
    /// </summary>
    /// <param name="classifier">The classifier.</param>
    /// <param name="trainingSetSplitRate">The training set split rate.</param>
    /// <param name="mutationRate">The mutation rate.</param>
    /// <param name="bitSwapRate">The bit swap rate.</param>
    /// <param name="preservationRate">The preservation rate.</param>
    /// <param name="generationsNumber">The generation number.</param>
    /// <param name="populationSizes">The population sizes.</param>
    /// <param name="initialFillups">The initial fillups.</param>
    /// <param name="numberOfCores">The number of cores.</param>
    /// <param name="seed">The seed.</param>
    /// <param name="minimalFillup">The minimal fillup.</param>
    /// <param name="maximalFillup">The maximal fillup.</param>
    GaClassifier(IClassifier& classifier,
        double trainingSetSplitRate,
        double mutationRate,
        double bitSwapRate,
        double preservationRate,
        unsigned int generationsNumber,
        unsigned int populationSize,
        unsigned int initialFillup,
        spectre::algorithm::genetic::Seed seed = 0,
        size_t minimalFillup = 1ul,
        size_t maximalFillup = std::numeric_limits<size_t>::max());
    /// <summary>
    /// Try to fit classifier to the data.
    /// </summary>
    /// <param name="dataset">The dataset.</param>
    /// <returns>void</returns>
    void Fit(LabeledDataset dataset) override;
    /// <summary>
    /// Predicts labels on test set.
    /// </summary>
    /// <param name="dataset">The dataset.</param>
    /// <returns>vector of labels</returns>
    std::vector<Label> Predict(LabeledDataset dataset) const override;
private:
    static const OpenCvDataset& asSupported(LabeledDataset);
    /// <summary>
    /// The population sizes.
    /// </summary>
    IClassifier& m_Classifier;
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
    /// The seed.
    /// </summary>
    const spectre::algorithm::genetic::Seed m_Seed;
    /// <summary>
    /// The number of generations.
    /// </summary>
    const unsigned int m_NumberOfGenerations;
    /// <summary>
    /// The genetic algorithm factory.
    /// </summary>
    const spectre::algorithm::genetic::GeneticAlgorithmFactory m_GaFactory;
    /// <summary>
    /// The minimal fillup.
    /// </summary>
    const size_t m_MinimalFillup;
    /// <summary>
    /// The maximal fillup.
    /// </summary>
    const size_t m_MaximalFillup;
};
}
