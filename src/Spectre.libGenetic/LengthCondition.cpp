/*
* MinimalLengthCondition.cpp
* Checks, if Individual is at least of specific length.
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

#include "LengthCondition.h"

namespace spectre::algorithm::genetic
{
LengthCondition::LengthCondition(size_t length, std::unique_ptr<BaseIndividualFeasibilityCondition> condition):
    BaseIndividualFeasibilityCondition(std::move(condition)),
    m_Length(length) {}

bool LengthCondition::checkCurrentCondition(const spectre::algorithm::genetic::Individual &individual)
{
    return individual.size() == m_Length;
}

}