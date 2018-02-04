/*
* GeneticAlgorithm.h
* Main algorithm class.
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
#include <memory>
#include "Spectre.libGenetic/Generation.h"
#include "Spectre.libGenetic/OffspringGenerator.h"
#include "Spectre.libGenetic/Scorer.h"
#include "Spectre.libGenetic/StopCondition.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// General-purpose genetic algorithm.
/// </summary>
class GeneticAlgorithm
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="GeneticAlgorithm"/> class.
    /// </summary>
    /// <param name="offspringGenerator">The offspring generator.</param>
    /// <param name="scorer">The scorer.</param>
    /// <param name="stopCondition">The stop condition.</param>
    /// <param name="individualFeasibilityConditions">The individual feasibility conditions.</param>
    GeneticAlgorithm(std::unique_ptr<OffspringGenerator> offspringGenerator, std::unique_ptr<Scorer> scorer, std::unique_ptr<StopCondition> stopCondition, 
                     std::unique_ptr<BaseIndividualFeasibilityCondition> individualFeasibilityConditions = nullptr);
    /// <summary>
    /// Evolves the specified generation.
    /// </summary>
    /// <param name="generation">The generation.</param>
    /// <returns>Next, evolved generation.</returns>
    Generation evolve(Generation &&generation) const;
    virtual ~GeneticAlgorithm() = default;

private:
    /// <summary>
    /// The offspring generator.
    /// </summary>
    std::unique_ptr<OffspringGenerator> m_OffspringGenerator;
    /// <summary>
    /// The scorer.
    /// </summary>
    std::unique_ptr<Scorer> m_Scorer;
    /// <summary>
    /// The stop condition.
    /// </summary>
    std::unique_ptr<StopCondition> m_StopCondition;
    /// <summary>
    /// The individual feasibility conditions.
    /// </summary>
    std::unique_ptr<BaseIndividualFeasibilityCondition> m_IndividualFeasibilityConditions;
};
}
