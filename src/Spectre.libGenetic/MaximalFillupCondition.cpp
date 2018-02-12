/*
* MaximalFillupCondition.cpp
* Checks, if Individual has at max specific amount of true values.
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

#include "MaximalFillupCondition.h"

namespace spectre::algorithm::genetic
{
MaximalFillupCondition::MaximalFillupCondition(size_t minimalFillup, std::unique_ptr<BaseIndividualFeasibilityCondition> condition) :
    BaseIndividualFeasibilityCondition(std::move(condition)),
    m_MaximalFillup(minimalFillup) {}

bool MaximalFillupCondition::currentConditionCheck(const spectre::algorithm::genetic::Individual &individual)
{
    auto number = 0u;
    for (bool value : individual.getData())
    {
        number += value;
        if (number <= m_MaximalFillup) return true;
    }
    return false;
}

}
