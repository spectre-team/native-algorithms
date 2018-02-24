/*
* IndividualFeasibilityConditionsFactory.h
* Creates individual feasibility condition chain.
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

namespace spectre::supervised
{
/// <summary>
/// Creates individual feasibility condition chain.
/// </summary>
class IndividualFeasibilityConditionsFactory
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="IndividualFeasibilityConditionsFactory"/> class.
    /// </summary>
    /// <param name="length">The length.</param>
    /// <param name="minimalFillup">The minimal fillup.</param>
    /// <param name="maximalFillup">The maximal fillup.</param>
    /// <param name="minimalPercentageFillup">The minimal percentage fillup.</param>
    /// <param name="maximalPercentageFillup">The maximal percentage fillup.</param>
    /// <param name="allLabelTypes">if all types of label have to be included.</param>
    IndividualFeasibilityConditionsFactory(size_t length = 0, size_t minimalFillup = 0, size_t maximalFillup = UINT_MAX, double minimalPercentageFillup = 0,
                                           double maximalPercentageFillup = 1, bool allLabelTypes = false, gsl::span<const supervised::Label> labels = nullptr);
    /// <summary>
    /// Creates Individual Feasibility Condition with given parameter values.
    /// </summary>
    /// <returns>Individual Feasibility Conditions chain.</returns>
    std::unique_ptr<spectre::algorithm::genetic::BaseIndividualFeasibilityCondition> build();
private:
    /// <summary>
    /// The length.
    /// </summary>
    size_t m_Length;
    /// <summary>
    /// The minimal fillup.
    /// </summary>
    size_t m_MinimalFillup;
    /// <summary>
    /// The maximal fillup.
    /// </summary>
    size_t m_MaximalFillup;
    /// <summary>
    /// The minimal percentage fillup.
    /// </summary>
    double m_MinimalPercentageFillup;
    /// <summary>
    /// The maximal percentage fillup.
    /// </summary>
    double m_MaximalPercentageFillup;
    /// <summary>
    /// If all label types have to be included.
    /// </summary>
    bool m_AllLabelTypes;
    /// <summary>
    /// If all label types have to be included.
    /// </summary>
    gsl::span<const supervised::Label> m_Labels;
};
}
