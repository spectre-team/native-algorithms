/*
* ClassifierFitnessFunction.cpp
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

#include "ClassifierFitnessFunction.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"
#include "Spectre.libFunctional/Filter.h"
#include "Spectre.libClassifier/ConfusionMatrix.h"
#include <memory>
#include "Spectre.libClassifier/DatasetFilter.h"
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libClassifier/DatasetFactory.h"

namespace spectre::supervised
{
ClassifierFitnessFunction::ClassifierFitnessFunction(RaportGenerator& raport, const IClassifier& classifier, SplittedOpenCvDataset& data,
    OpenCvDataset* independentValidation, std::string dataFilename, RaportGenerator* finalRaport):
    m_Raport(raport),
    m_Classifier(classifier),
    m_Data(data),
    m_IndependentValidation(independentValidation),
    m_FinalRaport(finalRaport),
    m_FullDataset(dataFilename)
{
}

algorithm::genetic::ScoreType ClassifierFitnessFunction::operator()(const algorithm::genetic::Individual& individual)
{
    if (m_Data.trainingSet.size() != individual.size())
    {
        throw core::exception::InconsistentArgumentSizesException("data", m_Data.trainingSet.size(), "individual", individual.size());
    }
    auto individualData = individual.getData();
    OpenCvDataset individualDataset = getFilteredOpenCvDataset(m_Data.trainingSet, individualData);

    const auto result = getResultMatrix(individualData, std::move(individualDataset));
    return result.DiceIndex;
}

ConfusionMatrix ClassifierFitnessFunction::getResultMatrix(std::vector<bool> individualData, const OpenCvDataset& data) const
{
    std::unique_ptr<IClassifier> classifierDuplicate = m_Classifier.clone();
    std::unique_ptr<OpenCvDataset> bestData = classifierDuplicate->Fit(data);
    const auto predictions = classifierDuplicate->Predict((m_Data.testSet));
    ConfusionMatrix confusions(predictions, m_Data.testSet.GetSampleMetadata());
    std::unique_ptr<ConfusionMatrix> validationConfusions;
    if (m_IndependentValidation != nullptr)
    {
        if (m_FullDataset != "")
        {
            DatasetFactory factory{};
            OpenCvDataset FullDataset = factory.create(m_FullDataset);
            std::vector<bool> finalSpectres({});
            for (auto i = 0u; i < FullDataset.size(); i++)
            {
                finalSpectres.push_back(false);
            }
            for (auto a = 0u; a < bestData->size(); a++)
            {
                for (auto b = 0u; b <FullDataset.size(); b++)
                {
                    for (auto c = 0u; c < FullDataset[b].size(); c++)
                    {
                        if (bestData->operator[](a)[c] != FullDataset[b][c]) break;
                        if (c == FullDataset[b].size() - 1) finalSpectres[b] = true;
                    }
                }
            }
            m_FinalRaport->WriteFinal(finalSpectres);
        }

        const auto validationPredictions = classifierDuplicate->Predict(*m_IndependentValidation);
        validationConfusions = std::make_unique<ConfusionMatrix>(validationPredictions, m_IndependentValidation->GetSampleMetadata());
    }
    m_Raport.Write(confusions, individualData, validationConfusions.get());
    return confusions;
}

}
