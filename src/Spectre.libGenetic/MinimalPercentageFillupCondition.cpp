/*
* MinimalPercentageFillupCondition.cpp
* Checks, if Individual has at least specific percentage amount of true values.
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

#include "MinimalPercentageFillupCondition.h"

namespace spectre::algorithm::genetic
{
MinimalPercentageFillupCondition::MinimalPercentageFillupCondition(double minimalPercentageFillup, std::unique_ptr<BaseIndividualFeasibilityCondition> condition) :
    BaseIndividualFeasibilityCondition(std::move(condition)),
    m_MinimalPercentageFillup(minimalPercentageFillup) {}

bool MinimalPercentageFillupCondition::checkCurrentCondition(const spectre::algorithm::genetic::Individual &individual)
{
    float number = 0;
    for (bool value : individual.getData())
    {
        number += value;
    }
    return (number / individual.size()) >= m_MinimalPercentageFillup;
}

}
