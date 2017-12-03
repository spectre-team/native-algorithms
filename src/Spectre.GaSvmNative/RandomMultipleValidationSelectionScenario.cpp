/*
* RandomMultipleValidationSelectionScenario.cpp
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

#include "RandomMultipleValidationSelectionScenario.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"
#include <omp.h>
#include "Spectre.libClassifier/DownsampledOpenCVDataset.h"
#include "Spectre.GaSvmNative/RaportGenerator.h"
#include "Spectre.libClassifier/RandomSplitter.h"
#include "Spectre.GaSvmNative/SVMFitnessFunction.h"

namespace Spectre::GaSvmNative
{
    RandomMultipleValidationSelectionScenario::RandomMultipleValidationSelectionScenario(double trainingSetSplitRate,
    double downsampleTrainingRate,
    size_t maximumSubsetSize,
    double mutationRate,
    double bitSwapRate,
    double preservationRate,
    unsigned int numberOfGenerations,
    unsigned int populationSize,
    unsigned int initialFillup,
    const std::string& filename,
    unsigned int numberOfRestarts,
    unsigned int numberOfCores,
    libGenetic::Seed seed,
    size_t minimalFillup,
    size_t maximalFillup,
    uint svmIterations,
    double svmTolerance) :
    m_PopulationSize(populationSize),
    m_InitialIndividualFillup(initialFillup),
    m_TrainingDatasetSizeRate(trainingSetSplitRate),
    m_DownsampleTrainingRate(downsampleTrainingRate),
    m_MaximumSubsetSize(maximumSubsetSize),
    m_Filename(filename),
    m_RestartsNumber(numberOfRestarts),
    m_NumberOfCores(numberOfCores),
    m_Seed(seed),
    m_GaFactory(mutationRate,
        bitSwapRate,
        preservationRate,
        numberOfGenerations,
        numberOfCores / numberOfRestarts > 0 ? numberOfCores / numberOfRestarts : 1u,
        minimalFillup,
        maximalFillup),
    m_SvmIterations(svmIterations),
    m_SvmTolerance(svmTolerance)
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

void RandomMultipleValidationSelectionScenario::execute(libClassifier::OpenCvDataset &data, const libClassifier::OpenCvDataset *independentValidation) const
{
    libClassifier::DownsampledOpenCVDataset downsampledOpenCvDataset(std::move(data), m_MaximumSubsetSize, m_DownsampleTrainingRate);

    const auto optimalChunksNumber = 1;
    omp_set_nested(1);
    #pragma omp parallel for schedule(dynamic, optimalChunksNumber) num_threads (m_NumberOfCores)
    for (auto runNumber = 0; runNumber < static_cast<int>(m_RestartsNumber); runNumber++)
    {
        RaportGenerator raportGenerator(m_Filename
            + "-run-" + std::to_string(runNumber)
            + "-popsize-" + std::to_string(m_PopulationSize)
            + "-fillup-" + std::to_string(m_InitialIndividualFillup),
            m_PopulationSize);

        libClassifier::SplittedOpenCvDataset splitterCancerNonCancerDataset = downsampledOpenCvDataset.getDownsizedOpenCVDataset(m_Seed + runNumber);

        //GA training
        libClassifier::RandomSplitter splitter(m_TrainingDatasetSizeRate, m_Seed + runNumber);
        auto splittedDataset = splitter.split(splitterCancerNonCancerDataset.trainingSet);
        auto trainingSetSize = splittedDataset.trainingSet.size();

        auto fitnessFunction = std::make_unique<SVMFitnessFunction>(splittedDataset,
            raportGenerator,
            independentValidation,
            m_SvmIterations,
            m_SvmTolerance);
        auto algorithm = m_GaFactory.BuildDefault(std::move(fitnessFunction), m_Seed + runNumber);

        libGenetic::Generation initialGeneration(m_PopulationSize, trainingSetSize, m_InitialIndividualFillup, m_Seed + runNumber);
        auto finalGeneration = algorithm->evolve(std::move(initialGeneration));

        //train and predict SVM on GA data
        RaportGenerator raportGeneratorGA("GA_" + m_Filename
            + "-run-" + std::to_string(runNumber)
            + "-popsize-" + std::to_string(m_PopulationSize)
            + "-fillup-" + std::to_string(m_InitialIndividualFillup),
            m_PopulationSize);
        auto GAFitnessFunction = std::make_unique<SVMFitnessFunction>(splitterCancerNonCancerDataset,
            raportGenerator,
            nullptr,
            m_SvmIterations,
            m_SvmTolerance);

        std::vector<bool> finalBinaryData = finalGeneration[0].getData();
        for (auto i = finalBinaryData.size(); i < splitterCancerNonCancerDataset.trainingSet.size(); i++)
        {
            finalBinaryData.push_back(false);
        }
        libGenetic::Individual finalGAIndividual(std::move(finalBinaryData));
        GAFitnessFunction->computeFitness(finalGAIndividual);

        //train and predict SVM on all downsampled data
        RaportGenerator raportGeneratorNonGA("NonGA_" + m_Filename
            + "-run-" + std::to_string(runNumber)
            + "-popsize-" + std::to_string(m_PopulationSize)
            + "-fillup-" + std::to_string(m_InitialIndividualFillup),
            m_PopulationSize);
        auto nonGAFitnessFunction = std::make_unique<SVMFitnessFunction>(splitterCancerNonCancerDataset,
            raportGenerator,
            nullptr,
            m_SvmIterations,
            m_SvmTolerance);

        libGenetic::Individual nonGAIndividual(splitterCancerNonCancerDataset.trainingSet.size(), splitterCancerNonCancerDataset.trainingSet.size(), 0);
        nonGAFitnessFunction->computeFitness(nonGAIndividual);
    }
}

}
