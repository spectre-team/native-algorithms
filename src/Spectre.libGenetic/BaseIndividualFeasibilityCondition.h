/*
* BaseIndividualFeasibilityCondition.h
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

#pragma once
#include "Individual.h"
#include <memory>

namespace spectre::algorithm::genetic
{
/// <summary>
/// Interface for Individual feasibility condition classes.
/// </summary>
class BaseIndividualFeasibilityCondition
{
protected:
    /// <summary>
    /// Initializes a new instance of the <see cref="BaseIndividualFeasibilityCondition"/> class.
    /// </summary>
    /// <param name="condition">The individual feasibility condition.</param>
    BaseIndividualFeasibilityCondition(std::unique_ptr<BaseIndividualFeasibilityCondition> condition);
    /// <summary>
    /// Unique individual check feasibility if it's correct.
    /// </summary>
    /// <param name="individual">The individual to check.</param>
    /// <returns>true if conditions are fulfilled by an individual.</returns>
    virtual bool currentConditionCheck(const Individual &individual) = 0;
public:
    /// <summary>
    /// Check individual feasibility if it's correct.
    /// </summary>
    /// <param name="individual">The individual to check.</param>
    /// <returns>bool</returns>
    bool check(const Individual &individual);
    virtual ~BaseIndividualFeasibilityCondition() = default;
private:
    /// <summary>
    /// The next individual feasibility condition.
    /// </summary>
    std::unique_ptr<BaseIndividualFeasibilityCondition> m_Next;
};
}
