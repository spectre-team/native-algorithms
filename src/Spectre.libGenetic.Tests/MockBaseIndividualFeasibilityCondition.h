/*
* MockBaseIndividualFeasibilityCondition.h
* Mock for a base individual feasibility condition.
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

#pragma once

#include <gmock/gmock.h>
#include "Spectre.libGenetic/BaseIndividualFeasibilityCondition.h"

namespace spectre::algorithm::genetic::Tests
{
class MockBaseIndividualFeasibilityCondition : public BaseIndividualFeasibilityCondition
{
public:
    MockBaseIndividualFeasibilityCondition(std::unique_ptr<BaseIndividualFeasibilityCondition> condition):
        BaseIndividualFeasibilityCondition(std::move(condition)) {}

    MOCK_METHOD1(checkCurrentCondition, bool(const Individual &individual));
private:
    std::unique_ptr<BaseIndividualFeasibilityCondition> base_individual_feasibility_condition;
};
}
