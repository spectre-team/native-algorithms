/*
* MutationOperator.h
* Mutates an individual.
*
Copyright 2017 Grzegorz Mrukwa, Wojciech Wilgierz

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
#include "Spectre.libGenetic/DataTypes.h"
#include "Spectre.libGenetic/Individual.h"
#include "Spectre.libGenetic/BaseIndividualFeasibilityCondition.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// Specifies, how to apply mutation on an individual.
/// </summary>
class MutationOperator
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="MutationOperator"/> class.
    /// </summary>
    /// <param name="mutationRate">The mutation rate.</param>
    /// <param name="bitSwapRate">The bit swap rate, in the case of mutation.</param>
    /// <param name="rngSeed">The RNG seed.</param>
    /// <param name="minimalFillup">The maximal fillup.</param>
    /// <param name="maximalFillup">The minimal fillup.</param>
    /// <param name="individualFeasibilityCondition">The individual feasibility condition.</param>
    explicit MutationOperator(double mutationRate, double bitSwapRate, Seed rngSeed = 0, BaseIndividualFeasibilityCondition* condition = nullptr);
    /// <summary>
    /// Mutates the specified individual until it matches conditions.
    /// </summary>
    /// <param name="individual">The individual.</param>
    /// <returns>Mutated individual fulfilling conditions. Operator mutates individual until all conditions are met</returns>
    virtual Individual operator()(Individual &&individual);
    /// <summary>
    /// Mutates the specified individual.
    /// </summary>
    /// <param name="individual">The individual.</param>
    /// <returns>Mutated individual.</returns>
    virtual Individual mutateWithoutConditions(Individual &&individual);
    virtual ~MutationOperator() = default;
private:
    /// <summary>
    /// The mutation rate.
    /// </summary>
    const double m_MutationRate;
    /// <summary>
    /// The bit swap rate.
    /// </summary>
    const double m_BitSwapRate;
    /// <summary>
    /// The random number generator.
    /// </summary>
    RandomNumberGenerator m_RandomNumberGenerator;
    /// <summary>
    /// The individual feasibility condition.
    /// </summary>
    BaseIndividualFeasibilityCondition* m_IndividualFeasibilityCondition;
};
}
