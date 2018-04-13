/*
* GeneticTrainingSetSelectionScenario.cpp
* Executes whole genetic algorithm and returns results
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

#include "GeneticTrainingSetSelectionScenario.h"
#include "Spectre.libClassifier/RaportGenerator.h"
#include "ClassifierFactory.h"
#include "Spectre.libClassifier/DatasetFactory.h"
#include "Spectre.libClassifier/DownsampledOpenCVDataset.h"
#include "ClassifierFitnessFunction.h"

namespace spectre::supervised
{
GeneticTrainingSetSelectionScenario::GeneticTrainingSetSelectionScenario(std::string name,
    double trainingSetSplitRate,
    double mutationRate,
    double bitSwapRate,
    double preservationRate,
    unsigned generationsNumber,
    unsigned populationSize,
    unsigned initialFillup,
    spectre::algorithm::genetic::Seed seed,
    size_t minimalFillup,
    size_t maximalFillup,
    unsigned int iterationsLimit,
    double tolerance) :
    m_Name(name),
    m_TrainingSetSplitRate(trainingSetSplitRate),
    m_MutationRate(mutationRate),
    m_BitSwapRate(bitSwapRate),
    m_PreservationRate(preservationRate),
    m_GenerationsNumber(generationsNumber),
    m_PopulationSize(populationSize),
    m_InitialFillup(initialFillup),
    m_Seed(seed),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup),
    m_IterationsLimit(iterationsLimit),
    m_Tolerance(tolerance)
{
}

void GeneticTrainingSetSelectionScenario::execute(std::string filename, std::string raportFilename, unsigned int populationSize) const
{
    ClassifierFactory classifierFactory{};
    DatasetFactory datasetFactory{};
    OpenCvDataset dataset = datasetFactory.create(filename);
    DownsampledOpenCVDataset downsampled(std::move(dataset), 1000000, 0.7);

    auto splittedOpenCvDataset = downsampled.getRandomSubset();
    Svm svm = classifierFactory.buildSvm(m_IterationsLimit, m_Tolerance);

    RaportGenerator m_RaportwoGA(raportFilename + "_without_GA"
        + "-popsize-" + std::to_string(populationSize), populationSize);
    ClassifierFitnessFunction fitnessFunction(m_RaportwoGA, svm, splittedOpenCvDataset);
    std::vector<bool> individualData{};
    for (auto i = 0u; i < splittedOpenCvDataset.trainingSet.size(); i++)
    {
        individualData.push_back(true);
    }
    algorithm::genetic::Individual individual(std::move(individualData));
    fitnessFunction.operator()(individual);

    RaportGenerator m_RaportGA(raportFilename + "_with_GA"
        + "-popsize-" + std::to_string(populationSize), populationSize);
    GaClassifier gaClassifier = classifierFactory.buildGaClassifier(m_RaportGA, m_Name, m_TrainingSetSplitRate, m_MutationRate, m_BitSwapRate, m_PreservationRate,
        m_GenerationsNumber, m_PopulationSize, m_InitialFillup, m_Seed, m_MinimalFillup, m_MaximalFillup, m_IterationsLimit, m_Tolerance);

    ClassifierFitnessFunction ff(m_RaportGA, gaClassifier, splittedOpenCvDataset);
    std::vector<bool> individualData2{};
    for (auto i = 0u; i < splittedOpenCvDataset.trainingSet.size(); i++)
    {
        individualData2.push_back(true);
    }
    algorithm::genetic::Individual individual2(std::move(individualData2));
    ff.operator()(individual2);
}
}
