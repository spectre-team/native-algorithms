/*
* AllLabelTypesIncludedCondition.cpp
* Checks, if Individual has at least specific amount of true values.
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

#include <span.h>
#include "AllLabelTypesIncludedCondition.h"
#include "Spectre.libClassifier/NotABinaryLabelException.h"

namespace spectre::scenario::gasvm
{
AllLabelTypesIncludedCondition::AllLabelTypesIncludedCondition(gsl::span<const supervised::Label> labels, std::unique_ptr<BaseIndividualFeasibilityCondition> condition) :
    BaseIndividualFeasibilityCondition(std::move(condition)),
    m_Labels(labels) {}

bool AllLabelTypesIncludedCondition::checkCurrentCondition(const spectre::algorithm::genetic::Individual &individual)
{
    auto valueSum = 0u;
    auto labelsAmount = 0u;
    for (auto i = 0u; i < individual.size(); ++i)
    {
        if (individual.getData()[i])
        {
            ++labelsAmount;
            valueSum += m_Labels[i];
        }
        if (valueSum != 0 && valueSum != labelsAmount) return true;
    }
    return false;
}

}
