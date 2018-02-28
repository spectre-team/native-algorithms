/*
* GaClassifier.cpp
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

#include "GaClassifier.h"
#include "Spectre.libClassifier/RandomSplitter.h"
#include "Spectre.libGenetic/GenerationFactory.h"
#include "IndividualFeasibilityConditionsFactory.h"
#include "GaFitnessFunction.h"

namespace spectre::supervised
{

GaClassifier::GaClassifier(std::unique_ptr<IClassifier> classifier,
                           OpenCvDataset& data,
                           double trainingSetSplitRate,
                           double mutationRate,
                           double bitSwapRate,
                           double preservationRate,
                           unsigned int numberOfGenerations,
                           unsigned int populationSize,
                           unsigned int initialFillup,
                           spectre::algorithm::genetic::Seed seed,
                           size_t minimalFillup,
                           size_t maximalFillup):
    m_Classifier(std::move(classifier)),
    m_PopulationSize(populationSize),
    m_InitialIndividualFillup(initialFillup),
    m_TrainingDatasetSizeRate(trainingSetSplitRate),
    m_Seed(seed),
    m_GaFactory(mutationRate,
        bitSwapRate,
        preservationRate,
        numberOfGenerations,
        1u),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup)
{
    RandomSplitter splitter(m_TrainingDatasetSizeRate, m_Seed);
    auto splittedDataset = splitter.split(data);
    auto trainingSetSize = splittedDataset.trainingSet.size();

    IndividualFeasibilityConditionsFactory conditionsFactory(splittedDataset.trainingSet.GetSampleMetadata(), splittedDataset.trainingSet.size(), m_MinimalFillup, m_MaximalFillup);
    auto conditions = conditionsFactory.build();

    auto fitnessFunction = std::make_unique<GaFitnessFunction>(std::move(classifier), splittedDataset);
    auto algorithm = m_GaFactory.BuildDefault(std::move(fitnessFunction), m_Seed, std::move(conditions));
    spectre::algorithm::genetic::GenerationFactory generationFactory(numberOfGenerations, trainingSetSize, initialFillup);
    spectre::algorithm::genetic::Generation initialGeneration = generationFactory(m_Seed);
    auto finalGeneration = algorithm->evolve(std::move(initialGeneration));
}

void GaClassifier::Fit(LabeledDataset dataset)
{
}

std::vector<Label> GaClassifier::Predict(LabeledDataset dataset) const
{
}

}
