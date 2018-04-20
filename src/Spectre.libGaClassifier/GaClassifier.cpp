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
#include "Spectre.libClassifier/UnsupportedDatasetTypeException.h"
#include "Spectre.libFunctional/Filter.h"
#include "ClassifierFitnessFunction.h"
#include "Spectre.libClassifier/DatasetFilter.h"

namespace spectre::supervised
{

GaClassifier::GaClassifier(RaportGenerator& raport,
                           std::unique_ptr<IClassifier> classifier,
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
    m_Raport(raport),
    m_Classifier(std::move(classifier)),
    m_PopulationSize(populationSize),
    m_InitialIndividualFillup(initialFillup),
    m_TrainingDatasetSizeRate(trainingSetSplitRate),
    m_MutationRate(mutationRate),
    m_BitSwapRate(bitSwapRate),
    m_PreservationRate(preservationRate),
    m_Seed(seed),
    m_NumberOfGenerations(numberOfGenerations),
    m_GaFactory(mutationRate,
        bitSwapRate,
        preservationRate,
        numberOfGenerations,
        1u),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup)
{
}

std::unique_ptr<OpenCvDataset> GaClassifier::Fit(LabeledDataset dataset)
{
    const auto& data = asSupported(dataset);
    RandomSplitter splitter(m_TrainingDatasetSizeRate, m_Seed);
    auto splittedDataset = splitter.split(data);
    auto trainingSetSize = splittedDataset.trainingSet.size();

    IndividualFeasibilityConditionsFactory conditionsFactory(splittedDataset.trainingSet.GetSampleMetadata(), trainingSetSize, m_MinimalFillup, m_MaximalFillup);
    auto conditions = conditionsFactory.build();

    auto fitnessFunction = std::make_unique<ClassifierFitnessFunction>(m_Raport, *m_Classifier, splittedDataset);
    auto algorithm = m_GaFactory.BuildDefault(std::move(fitnessFunction), m_Seed, std::move(conditions));
    algorithm::genetic::GenerationFactory generationFactory(m_PopulationSize, trainingSetSize, m_InitialIndividualFillup);
    algorithm::genetic::Generation initialGeneration = generationFactory(m_Seed);
    auto finalGeneration = algorithm->evolve(std::move(initialGeneration));
    auto bestIndividual = finalGeneration[0];

    auto individualData = bestIndividual.getData();
    std::unique_ptr<OpenCvDataset> result = std::make_unique<OpenCvDataset>(getFilteredOpenCvDataset(splittedDataset.trainingSet, individualData));
    OpenCvDataset bestDataset = getFilteredOpenCvDataset(splittedDataset.trainingSet, individualData);

    m_Classifier->Fit(bestDataset);
    return result;
}

std::vector<Label> GaClassifier::Predict(LabeledDataset dataset) const
{
    return m_Classifier->Predict(dataset);
}

std::unique_ptr<IClassifier> GaClassifier::clone() const
{
    return std::make_unique<GaClassifier>(m_Raport, m_Classifier->clone(), m_TrainingDatasetSizeRate, m_MutationRate, m_BitSwapRate, m_PreservationRate,
        m_NumberOfGenerations, m_PopulationSize, m_InitialIndividualFillup, m_Seed, m_MinimalFillup, m_MaximalFillup);
}

const OpenCvDataset& GaClassifier::asSupported(LabeledDataset dataset)
{
    try
    {
        const auto& casted = dynamic_cast<const OpenCvDataset&>(dataset);
        return casted;
    }
    catch (const std::bad_cast&)
    {
        throw exception::UnsupportedDatasetTypeException(dataset);
    }
}

}
