/*
* BaseIndividualFeasibilityCondition.cpp
* Base class for Individual feasibility condition classes.
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

#include "BaseIndividualFeasibilityCondition.h"

namespace spectre::algorithm::genetic
{
BaseIndividualFeasibilityCondition::BaseIndividualFeasibilityCondition(std::unique_ptr<BaseIndividualFeasibilityCondition> nextCondition)
{
    m_NextCondition = std::move(nextCondition);
}

bool BaseIndividualFeasibilityCondition::check(const Individual &individual)
{
    if (!this->checkCurrentCondition(individual))
    {
        return false;
    }
    if (m_NextCondition == nullptr)
    {
        return true;
    }
    return m_NextCondition->check(individual);
}

}
