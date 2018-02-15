/*
* SVMFitnessFunction.cpp
* Trains data in svm and returns score of a Individual.
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

#include <ctime>
#include "SVMFitnessFunction.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"
#include "Spectre.libFunctional/Filter.h"
#include <memory>

namespace spectre::algorithm::genetic
{
SVMFitnessFunction::SVMFitnessFunction(supervised::SplittedOpenCvDataset&& data,
                                       scenario::gasvm::RaportGenerator& raportGenerator,
                                       const spectre::supervised::OpenCvDataset* independentValidation,
                                       unsigned int svmIterations,
                                       double svmTolerance)
    : m_Dataset(std::move(data)),
      m_IndependentValidation(independentValidation),
      m_RaportGenerator(raportGenerator),
      m_SvmIterations(svmIterations),
      m_SvmTolerance(svmTolerance)
{
}

ScoreType SVMFitnessFunction::operator()(const Individual &individual)
{
    if (m_Dataset.trainingSet.size() != individual.size())
    {
        throw core::exception::InconsistentArgumentSizesException("data", m_Dataset.trainingSet.size(), "individual", individual.size());
    }

    const auto& dataToFilter = m_Dataset.trainingSet.GetData();
    const auto twoDimensionalFilteredData = spectre::core::functional::filter(dataToFilter, individual.getData());
    std::vector<spectre::supervised::DataType> oneDimensionalFilteredData;
    oneDimensionalFilteredData.reserve(twoDimensionalFilteredData.size() * twoDimensionalFilteredData[0].size());
    for (const auto& observation: twoDimensionalFilteredData)
    {
        oneDimensionalFilteredData.insert(oneDimensionalFilteredData.end(), observation.begin(), observation.end());
    }
    const auto filteredLabels = spectre::core::functional::filter(m_Dataset.trainingSet.GetSampleMetadata(), individual.getData());
    supervised::OpenCvDataset individualDataset(oneDimensionalFilteredData, filteredLabels);

    const auto result = getResultMatrix(std::move(individualDataset), individual);

    return result.DiceIndex;
}

supervised::ConfusionMatrix SVMFitnessFunction::getResultMatrix(const supervised::OpenCvDataset& data, const Individual& individual) const
{
    supervised::Svm svm(m_SvmIterations, m_SvmTolerance);
    const auto trainingBegin = clock();
    svm.Fit(data);
    const auto trainingEnd = clock();
    const auto trainingTime = double(trainingEnd - trainingBegin) / CLOCKS_PER_SEC;

    const auto classificationBegin = clock();
    const auto predictions = svm.Predict((m_Dataset.testSet));
    const auto classificationEnd = clock();
    const auto classificationTime = double(classificationEnd - classificationBegin) / CLOCKS_PER_SEC;
    supervised::ConfusionMatrix confusions(predictions, m_Dataset.testSet.GetSampleMetadata());

    std::unique_ptr<supervised::ConfusionMatrix> validationConfusions;
    if (m_IndependentValidation != nullptr)
    {
        const auto validationPredictions = svm.Predict(*m_IndependentValidation);
        validationConfusions = std::make_unique<supervised::ConfusionMatrix>(validationPredictions, m_IndependentValidation->GetSampleMetadata());
    }

    m_RaportGenerator.Write(confusions,
                            individual,
                            trainingTime,
                            classificationTime / m_Dataset.testSet.size(),
                            svm.GetNumberOfSupportVectors(),
                            validationConfusions.get());

    return confusions;
}
}
