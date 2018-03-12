/*
* MinimalPercentageFillupCondition.h
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

#pragma once
#include "Spectre.libGenetic/BaseIndividualFeasibilityCondition.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// Checks, if Individual has at least specific percentage amount of true values.
/// </summary>
class MinimalPercentageFillupCondition : public BaseIndividualFeasibilityCondition
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="MinimalPercentageFillupCondition"/> class.
    /// </summary>
    /// <param name="minimalPercentageFillup">The minimal percentage fillup.</param>
    /// <param name="condition">The individual feasibility condition.</param>
    MinimalPercentageFillupCondition(double minimalPercentageFillup, std::unique_ptr<BaseIndividualFeasibilityCondition> condition = nullptr);
    /// <summary>
    /// Check individual feasibility if it's correct.
    /// </summary>
    /// <param name="individual">The individual to check.</param>
    /// <returns>true if conditions are fulfilled by an individual.</returns>
    bool checkCurrentCondition(const Individual &individual) override;
private:
    /// <summary>
    /// The minimal percentage fillup.
    /// </summary>
    double m_MinimalPercentageFillup;
};
}
