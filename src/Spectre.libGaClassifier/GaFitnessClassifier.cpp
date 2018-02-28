/*
* GaFitnessFunction.cpp
* It is fitness function for any classifier for GaClassifier.
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

#include "GaFitnessFunction.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"
#include "Spectre.libFunctional/Filter.h"
#include "Spectre.libClassifier/ConfusionMatrix.h"

namespace spectre::supervised
{

GaFitnessFunction::GaFitnessFunction(std::unique_ptr<IClassifier> classifier, SplittedOpenCvDataset& data):
    m_Classifier(std::move(classifier)),
    m_Data(data)
{
}

algorithm::genetic::ScoreType GaFitnessFunction::operator()(const algorithm::genetic::Individual& individual)
{
    if (m_Data.trainingSet.size() != individual.size())
    {
        throw core::exception::InconsistentArgumentSizesException("data", m_Data.trainingSet.size(), "individual", individual.size());
    }

    const auto& dataToFilter = m_Data.trainingSet.GetData();
    const auto twoDimensionalFilteredData = core::functional::filter(dataToFilter, individual.getData());
    std::vector<DataType> oneDimensionalFilteredData;
    oneDimensionalFilteredData.reserve(twoDimensionalFilteredData.size() * twoDimensionalFilteredData[0].size());
    for (const auto& observation : twoDimensionalFilteredData)
    {
        oneDimensionalFilteredData.insert(oneDimensionalFilteredData.end(), observation.begin(), observation.end());
    }
    const auto filteredLabels = core::functional::filter(m_Data.trainingSet.GetSampleMetadata(), individual.getData());
    OpenCvDataset individualDataset(oneDimensionalFilteredData, filteredLabels);

    const auto result = getResultMatrix(std::move(individualDataset));
    return result.DiceIndex;
}

ConfusionMatrix GaFitnessFunction::getResultMatrix(const OpenCvDataset& data) const
{
    m_Classifier->Fit(data);
    const auto predictions = m_Classifier->Predict((m_Data.testSet));
    ConfusionMatrix confusions(predictions, m_Data.testSet.GetSampleMetadata());
    return confusions;
}

}
