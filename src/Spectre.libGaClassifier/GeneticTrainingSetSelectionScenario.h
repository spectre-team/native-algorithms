/*
* GeneticTrainingSetSelectionScenario.h
* Executes whole genetic algorithm and returns results.
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
#include "Spectre.libGenetic/DataTypes.h"
#include "Spectre.libClassifier/RaportGenerator.h"

namespace spectre::supervised
{
    class OpenCvDataset;

    class GeneticTrainingSetSelectionScenario
{
public:
    GeneticTrainingSetSelectionScenario(std::string name,
        double trainingSetSplitRate,
        double mutationRate,
        double bitSwapRate,
        double preservationRate,
        unsigned generationsNumber,
        unsigned populationSize,
        unsigned initialFillup,
        spectre::algorithm::genetic::Seed seed,
        size_t minimalFillup = 1ul,
        size_t maximalFillup = std::numeric_limits<size_t>::max(),
        unsigned int iterationsLimit = 100,
        double tolerance = 1e-6);
        void execute(std::string filename, std::string raportFilename, unsigned int populationSize) const;
private:
    std::string m_Name;
    double m_TrainingSetSplitRate;
    double m_MutationRate;
    double m_BitSwapRate;
    double m_PreservationRate;
    unsigned m_GenerationsNumber;
    unsigned m_PopulationSize;
    unsigned m_InitialFillup;
    spectre::algorithm::genetic::Seed m_Seed;
    size_t m_MinimalFillup;
    size_t m_MaximalFillup;
    unsigned int m_IterationsLimit;
    double m_Tolerance;
};
}
