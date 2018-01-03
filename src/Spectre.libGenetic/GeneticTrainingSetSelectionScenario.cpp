﻿/*
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
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libClassifier/RandomSplitter.h"
#include "Spectre.libClassifier/SVMFitnessFunction.h"
#include "Spectre.libGenetic/GeneticAlgorithm.h"
#include "Spectre.libGenetic/RaportGenerator.h"
#include <ctime>
#include "Spectre.libGenetic/GeneticAlgorithmFactory.h"
#include <omp.h>
#include "Spectre.libException/ArgumentOutOfRangeException.h"

namespace Spectre::libGenetic
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
                                                                             Seed seed) :
        m_GenerationSizes(populationSize.begin(), populationSize.end()),
        m_InitialIndividualFillups(initialFillup.begin(), initialFillup.end()),
        m_TrainingDatasetSizeRate(trainingSetSplitRate),
        m_Filename(filename),
        m_RestartsNumber(numberOfRestarts),
        m_NumberOfCores(numberOfCores),
        m_Seed(seed),
        m_GaFactory(mutationRate, bitSwapRate, preservationRate, numberOfGenerations, numberOfCores / numberOfRestarts > 0 ? numberOfCores / numberOfRestarts : 1u)
    {
        if (m_NumberOfCores != 0)
        {
            // @gmrukwa: usual empty execution branch
        }
        else
        {
            throw libException::ArgumentOutOfRangeException<unsigned>("numberOfCores", 1, omp_get_num_procs(), m_NumberOfCores);
        }
    }

    void GeneticTrainingSetSelectionScenario::execute(libClassifier::OpenCvDataset data) const
    {
        const auto optimalChunksNumber = 1;
        omp_set_nested(1);
        #pragma omp parallel for schedule(dynamic, optimalChunksNumber) num_threads (m_NumberOfCores)
        for (auto runNumber = 0; runNumber < static_cast<int>(m_RestartsNumber); runNumber++)
        {
            for (auto popSize : m_GenerationSizes)
            {
                for (auto trueAmount : m_InitialIndividualFillups)
                {
                    auto begin = clock();
                    RaportGenerator raportGenerator(m_Filename + "_" + std::to_string(popSize) + "_" + std::to_string(trueAmount));
                    raportGenerator.write("Genetic algorithm " + std::to_string(i) + " run with " + std::to_string(popSize) + " population size and " + std::to_string(trueAmount) + "for true amount in individuals in initial generation.");
                    //raportGenerator.write("Data:");
                    //raportGenerator.write(&data);

                    libClassifier::RandomSplitter splitter(m_TrainingDatasetSizeRate, m_Seed + runNumber);
                    auto splittedDataset = splitter.split(data);
                    auto trainingSetSize = splittedDataset.trainingSet.size();
                    auto fitnessFunction = std::make_unique<libClassifier::SVMFitnessFunction>(std::move(splittedDataset), raportGenerator);
                    auto algorithm = m_GaFactory.BuildDefault(std::move(fitnessFunction), m_Seed + runNumber);

                    Generation initialGeneration(popSize, trainingSetSize, trueAmount);
                    auto finalGeneration = algorithm->evolve(std::move(initialGeneration));

                    auto end = clock();
                    auto elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
                    raportGenerator.write("Time needed for Genetic algorithm - " + std::to_string(runNumber) + " number of execution for parameters " + std::to_string(popSize) + " population size and " +
                        std::to_string(trueAmount) + "for true amount in individuals in initial generation is: " + std::to_string(elapsed_secs) + " seconds.");
                    raportGenerator.close();
                }
            }
        }
    }

}
