/*
* AllLabelTypesIncludedCondition.h
* Checks, if Individual includes all types of available labels.
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
#include "Spectre.libClassifier/NotABinaryLabelException.h"
#include <span.h>

namespace spectre::supervised
{
/// <summary>
/// Checks, if Individual includes all types of available labels.
/// </summary>
class AllLabelTypesIncludedCondition : public algorithm::genetic::BaseIndividualFeasibilityCondition
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="AllLabelTypesIncludedCondition"/> class.
    /// </summary>
    /// <param name="labels">The labels.</param>
    /// <param name="condition">The next individual feasibility condition.</param>
    AllLabelTypesIncludedCondition(gsl::span<const supervised::Label> labels, std::unique_ptr<BaseIndividualFeasibilityCondition> condition = nullptr);
    /// <summary>
    /// Check individual feasibility if it's correct.
    /// </summary>
    /// <param name="individual">The individual to check.</param>
    /// <returns>true if conditions are fulfilled by an individual.</returns>
    bool checkCurrentCondition(const algorithm::genetic::Individual &individual) override;
private:
    /// <summary>
    /// The labels.
    /// </summary>
    gsl::span<const supervised::Label> m_Labels;
    /// <summary>
    /// The amount of label types.
    /// </summary>
    const size_t m_LabelTypesAmount;
};
}
