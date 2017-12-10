/*
* SplittedOpenCvDataset.cpp
* Pair of OpenCvDatasets - training and test
*
Copyright 2017 Grzegorz Mrukwa, Wojciech Wilgierz

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

#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"

namespace Spectre::libClassifier {
SplittedOpenCvDataset::SplittedOpenCvDataset(OpenCvDataset&& training, OpenCvDataset&& test)
    : m_TrainingSet(std::move(training)),
      m_TestSet(std::move(test)),
      m_Indexes({})
{
    
}

SplittedOpenCvDataset::SplittedOpenCvDataset(OpenCvDataset &&training, OpenCvDataset &&test, std::vector<int> indexes)
    : m_TrainingSet(std::move(training)),
      m_TestSet(std::move(test)),
      m_Indexes(std::move(indexes))
{
    if (m_TrainingSet.size() + m_TestSet.size() != m_Indexes.size())
    {
        throw libException::InconsistentArgumentSizesException("training + test", m_TrainingSet.size() + m_TestSet.size(), "indexes", m_Indexes.size());
    }
}

std::vector<bool> SplittedOpenCvDataset::getWholeDatasetBinaryDataFromTrainingDatasetBinaryData(const std::vector<bool> trainingBinaryData) const
{
    if (trainingBinaryData.size() != m_TrainingSet.size())
    {
        throw libException::InconsistentArgumentSizesException("training binary data", trainingBinaryData.size(), "training set", m_TrainingSet.size());
    }

    std::vector<bool> wholeDatasetData{};
    wholeDatasetData.reserve(m_Indexes.size());
    for (auto i = 0; i < m_Indexes.size(); i++)
    {
        wholeDatasetData.push_back(false);
    }
    for (auto i = 0; i <  m_TrainingSet.size(); i++)
    {
        wholeDatasetData[m_Indexes[i]] = trainingBinaryData[i];
    }
    return wholeDatasetData;
}


}
