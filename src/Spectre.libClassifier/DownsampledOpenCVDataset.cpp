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
    std::vector<DataType> positiveLabeledData;
    std::vector<DataType> negativeLabeledData;
    std::vector<Label> positiveLabels;
    std::vector<Label> negativeLabels;
    auto positiveLabel = 1;
    auto negativeLabel = 0;
    gsl::span<const Label> labels = data.GetSampleMetadata();
    for (auto i = 0u; i < data.size(); i++)
    {
        if (labels[i] == positiveLabel)
        {
            for (DataType dataType : data[i]) {
                positiveLabeledData.push_back(dataType);
            }
            positiveLabels.push_back(labels(i));
        }
        else if (labels[i] == negativeLabel)
        {
            for (DataType dataType : data[i]) {
                negativeLabeledData.push_back(dataType);
            }
            negativeLabels.push_back(labels[i]);
        }
    }
    m_PositiveLabeledData = std::make_unique<OpenCvDataset>(positiveLabeledData, positiveLabels);
    m_NegativeLabeledData = std::make_unique<OpenCvDataset>(negativeLabeledData, negativeLabels);
    m_MaximumSubsetSize = std::min(maximumSubsetSize, std::min(m_PositiveLabeledData->size(), m_NegativeLabeledData->size()));
    m_PositiveDataExtractor = ObservationExtractor(m_PositiveLabeledData.get());
    m_NegativeDataExtractor = ObservationExtractor(m_NegativeLabeledData.get());
}

SplittedOpenCvDataset DownsampledOpenCVDataset::getRandomSubset(Seed seed) const
{
    //gets individual data for positive and negative data with the same amount of shuffled true values
    std::vector<bool> positiveLabeledBinaryData = getRandomFilter(m_PositiveLabeledData->size(), seed);
    std::vector<bool> negativeLabeledBinaryData = getRandomFilter(m_NegativeLabeledData->size(), seed);

    OpenCvDataset positiveLabeledDataset = m_PositiveDataExtractor.getOpenCvDatasetFromObservations(positiveLabeledBinaryData);
    OpenCvDataset negativeLabeledDataset = m_NegativeDataExtractor.getOpenCvDatasetFromObservations(negativeLabeledBinaryData);

    RandomSplitter randomSplitter(m_TrainingRate, seed);
    SplittedOpenCvDataset positiveLabeledSplitted = randomSplitter.split(positiveLabeledDataset);
    SplittedOpenCvDataset negativeLabeledSplitted = randomSplitter.split(negativeLabeledDataset);

    OpenCvDataset trainingDataset(std::move(positiveLabeledSplitted.trainingSet), std::move(negativeLabeledSplitted.trainingSet));
    OpenCvDataset testDataset(std::move(positiveLabeledSplitted.testSet), std::move(negativeLabeledSplitted.testSet));
    SplittedOpenCvDataset result(std::move(trainingDataset), std::move(testDataset));
    return result;
}

std::vector<bool> DownsampledOpenCVDataset::getRandomFilter(size_t datasetSize, Seed seed) const
{
    std::vector<bool> filter(datasetSize, false);
    for (auto i = 0u; i < m_MaximumSubsetSize; i++)
    {
        filter[i] = true;
    }
    RandomNumberGenerator rng(seed);
    std::shuffle(filter.begin(), filter.end(), rng);
    return filter;
}
}
