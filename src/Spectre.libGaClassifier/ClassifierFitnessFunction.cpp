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

namespace spectre::supervised
{
ClassifierFitnessFunction::ClassifierFitnessFunction(const IClassifier& classifier, SplittedOpenCvDataset& data):
    m_Classifier(classifier),
    m_Data(data)
{
}

algorithm::genetic::ScoreType ClassifierFitnessFunction::operator()(const algorithm::genetic::Individual& individual)
{
    if (m_Data.trainingSet.size() != individual.size())
    {
        throw core::exception::InconsistentArgumentSizesException("data", m_Data.trainingSet.size(), "individual", individual.size());
    }
    OpenCvDataset individualDataset = getFilteredOpenCvDataset(m_Data.trainingSet, individual);

    const auto result = getResultMatrix(std::move(individualDataset));
    return result.DiceIndex;
}

ConfusionMatrix ClassifierFitnessFunction::getResultMatrix(const OpenCvDataset& data) const
{
    std::unique_ptr<IClassifier> classifierDuplicate = m_Classifier.clone();
    classifierDuplicate->Fit(data);
    const auto predictions = classifierDuplicate->Predict((m_Data.testSet));
    ConfusionMatrix confusions(predictions, m_Data.testSet.GetSampleMetadata());
    return confusions;
}

}
