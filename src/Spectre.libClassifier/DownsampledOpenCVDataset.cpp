/*
* DownsampledOpenCVDataset.cpp
* Structure having same amount of cancer and noncancer cells for fitness function random multiple validation
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

#include "DownsampledOpenCVDataset.h"
#include "RandomSplitter.h"
#include "ObservationExtractor.h"
#include "Spectre.libGenetic/DataTypes.h"

namespace spectre::supervised
{
DownsampledOpenCVDataset::DownsampledOpenCVDataset(OpenCvDataset data, size_t maximumSubsetSize, double trainingRate)
    : m_TrainingRate(trainingRate)
{
    std::vector<DataType> cancerData{};
    std::vector<DataType> nonCancerData{};
    std::vector<Label> cancerLabels{};
    std::vector<Label> nonCancerLabels{};
    for (auto i = 0u; i < data.size(); i++)
    {
        if (data.GetSampleMetadata(i) == 1)
        {
            for (DataType dataType : data[i]) {
                cancerData.push_back(dataType);
            }
            cancerLabels.push_back(data.GetSampleMetadata(i));
        }
        else if (data.GetSampleMetadata(i) == 0)
        {
            for (DataType dataType : data[i]) {
                nonCancerData.push_back(dataType);
            }
            nonCancerLabels.push_back(data.GetSampleMetadata(i));
        }
    }
    m_CancerCells = std::make_unique<OpenCvDataset>(cancerData, cancerLabels);
    m_NonCancerCells = std::make_unique<OpenCvDataset>(nonCancerData, nonCancerLabels);
    if (maximumSubsetSize > m_CancerCells->size())
    {
        maximumSubsetSize = m_CancerCells->size();
    }
    if (maximumSubsetSize > m_NonCancerCells->size())
    {
        maximumSubsetSize = m_NonCancerCells->size();
    }
    m_MaximumSubsetSize = maximumSubsetSize;
}

SplittedOpenCvDataset DownsampledOpenCVDataset::getLimitedDownSampledOpenCVDataset(Seed seed) const
{
    std::vector<bool> cancerIndividualData = getBinaryDataWithGivenTrueValueAmount(m_CancerCells->size(), seed);
    std::vector<bool> nonCancerIndividualData = getBinaryDataWithGivenTrueValueAmount(m_NonCancerCells->size(), seed);

    ObservationExtractor cancerExtractor(m_CancerCells.get());
    OpenCvDataset cancerDataset = cancerExtractor.getOpenCvDatasetFromObservations(cancerIndividualData);
    ObservationExtractor nonCancerExtractor(m_NonCancerCells.get());
    OpenCvDataset nonCancerDataset = nonCancerExtractor.getOpenCvDatasetFromObservations(nonCancerIndividualData);

    RandomSplitter randomSplitter(m_TrainingRate, seed);
    SplittedOpenCvDataset cancerSplitted = randomSplitter.split(cancerDataset);
    SplittedOpenCvDataset nonCancerSplitted = randomSplitter.split(nonCancerDataset);

    OpenCvDataset trainingDataset(std::move(cancerSplitted.trainingSet), std::move(nonCancerSplitted.trainingSet));
    OpenCvDataset testDataset(std::move(cancerSplitted.testSet), std::move(nonCancerSplitted.testSet));
    SplittedOpenCvDataset result(std::move(trainingDataset), std::move(testDataset));
    return result;
}

std::vector<bool> DownsampledOpenCVDataset::getBinaryDataWithGivenTrueValueAmount(size_t datasetSize, Seed seed) const
{
    std::vector<bool> trainingData;
    trainingData.reserve(datasetSize);
    for (auto i = 0u; i < m_MaximumSubsetSize; i++)
    {
        trainingData.push_back(true);
    }
    for (auto i = m_MaximumSubsetSize; i < datasetSize; i++)
    {
        trainingData.push_back(false);
    }
    RandomNumberGenerator rng(seed);
    std::shuffle(trainingData.begin(), trainingData.end(), rng);
    return trainingData;
}
}
