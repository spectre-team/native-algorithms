/*
* GeneticTrainingSetSelectionScenario.cpp
* Executes whole genetic algorithm and returns results
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

#include <omp.h>
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libClassifier/RandomSplitter.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"
#include "Spectre.libGenetic/GeneticAlgorithmFactory.h"
#include "GeneticTrainingSetSelectionScenario.h"
#include "RaportGenerator.h"
#include "SVMFitnessFunction.h"
#include "Spectre.libGenetic/GenerationFactory.h"
#include "Spectre.libGenetic/MinimalFillupCondition.h"
#include "Spectre.libGenetic/MaximalFillupCondition.h"
#include "AllLabelTypesIncludedCondition.h"
#include "Spectre.libGenetic/LengthCondition.h"
#include "InconsistentMinimalAndMaximalFillupException.h"

namespace spectre::scenario::gasvm
{
GeneticTrainingSetSelectionScenario::GeneticTrainingSetSelectionScenario(double trainingSetSplitRate,
                                                                         double mutationRate,
                                                                         double bitSwapRate,
                                                                         double preservationRate,
                                                                         unsigned int numberOfGenerations,
                                                                         const std::vector<unsigned int>& populationSize,
                                                                         const std::vector<unsigned int>& initialFillup,
                                                                         const std::string& filename,
                                                                         unsigned int numberOfRestarts,
                                                                         unsigned int numberOfCores,
                                                                         spectre::algorithm::genetic::Seed seed,
                                                                         size_t minimalFillup,
                                                                         size_t maximalFillup,
                                                                         uint svmIterations,
                                                                         double svmTolerance) :
    m_PopulationSizes(populationSize.begin(), populationSize.end()),
    m_InitialIndividualFillups(initialFillup.begin(), initialFillup.end()),
    m_TrainingDatasetSizeRate(trainingSetSplitRate),
    m_Filename(filename),
    m_RestartsNumber(numberOfRestarts),
    m_NumberOfCores(numberOfCores),
    m_Seed(seed),
    m_GaFactory(mutationRate,
                bitSwapRate,
                preservationRate,
                numberOfGenerations,
                numberOfCores / numberOfRestarts > 0 ? numberOfCores / numberOfRestarts : 1u),
    m_SvmIterations(svmIterations),
    m_SvmTolerance(svmTolerance),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup)
{
    if (m_MinimalFillup > m_MaximalFillup)
    {
        throw InconsistentMinimalAndMaximalFillupException(m_MinimalFillup, m_MaximalFillup);
    }
    if (m_NumberOfCores == 0)
    {
        throw spectre::core::exception::ArgumentOutOfRangeException<unsigned>("numberOfCores", 1, omp_get_num_procs(), m_NumberOfCores);
    }
    for (auto fillup : m_InitialIndividualFillups)
    {
        if (fillup > m_MaximalFillup || fillup < m_MinimalFillup)
        {
            throw core::exception::ArgumentOutOfRangeException<size_t>("initialFillup", m_MinimalFillup, m_MaximalFillup, fillup);
        }
    }
}

void GeneticTrainingSetSelectionScenario::execute(const spectre::supervised::OpenCvDataset& data,
                                                  const spectre::supervised::OpenCvDataset* independentValidation) const
{
    const auto optimalChunksNumber = 1;
    omp_set_nested(1);
    #pragma omp parallel for schedule(dynamic, optimalChunksNumber) num_threads (m_NumberOfCores)
    for (auto runNumber = 0; runNumber < static_cast<int>(m_RestartsNumber); runNumber++)
    {
        for (auto popSize : m_PopulationSizes)
        {
            for (auto initialFillup : m_InitialIndividualFillups)
            {
                RaportGenerator raportGenerator(m_Filename
                                                + "-run-" + std::to_string(runNumber)
                                                + "-popsize-" + std::to_string(popSize)
                                                + "-fillup-" + std::to_string(initialFillup),
                                                popSize);

                spectre::supervised::RandomSplitter splitter(m_TrainingDatasetSizeRate, m_Seed + runNumber);
                auto splittedDataset = splitter.split(data);
                auto trainingSetSize = splittedDataset.trainingSet.size();

                auto allLabelCondition = std::make_unique<AllLabelTypesIncludedCondition>(splittedDataset.trainingSet.GetSampleMetadata());
                auto minimalFillupCondition = std::make_unique<algorithm::genetic::MinimalFillupCondition>(m_MinimalFillup, std::move(allLabelCondition));
                auto maximalFillupCondition = std::make_unique<algorithm::genetic::MaximalFillupCondition>(m_MaximalFillup, std::move(minimalFillupCondition));
                auto individualFeasibilityConditions = std::make_unique<algorithm::genetic::LengthCondition>(splittedDataset.trainingSet.size(), std::move(maximalFillupCondition));

                auto fitnessFunction = std::make_unique<algorithm::genetic::SVMFitnessFunction>(std::move(splittedDataset),
                                                                            raportGenerator,
                                                                            independentValidation,
                                                                            m_SvmIterations,
                                                                            m_SvmTolerance);
                auto algorithm = m_GaFactory.BuildDefault(std::move(fitnessFunction), m_Seed + runNumber, std::move(individualFeasibilityConditions));
                spectre::algorithm::genetic::GenerationFactory generationFactory(popSize, trainingSetSize, initialFillup);
                spectre::algorithm::genetic::Generation initialGeneration = generationFactory(m_Seed + runNumber);
                auto finalGeneration = algorithm->evolve(std::move(initialGeneration));
            }
        }
    }
}
}
