/*
* OffspringGenerator.h
* Describes how to create offspring from best and new individuals.
*
Copyright 2017 Grzegorz Mrukwa

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
#include <span.h>
#include "Spectre.libGenetic/DataTypes.h"
#include "Spectre.libGenetic/Generation.h"
#include "Spectre.libGenetic/IndividualsBuilderStrategy.h"
#include "Spectre.libGenetic/PreservationStrategy.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// Specifies, how to create new generation from the old one, basing on scores of each individual.
/// </summary>
class OffspringGenerator
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="OffspringGenerator"/> class.
    /// </summary>
    /// <param name="builder">The strategy specifying, how to build new individuals.</param>
    /// <param name="selection">The strategy specifying, how to select preserved individuals.</param>
    OffspringGenerator(std::unique_ptr<IndividualsBuilderStrategy> builder, std::unique_ptr<PreservationStrategy> preservationStrategy);
    /// <summary>
    /// Generates offspring from current population.
    /// </summary>
    /// <param name="old">The current generation.</param>
    /// <param name="scores">The scores of the individuals.</param>
    /// <returns>New generation.</returns>
    virtual Generation next(Generation &old, gsl::span<const ScoreType> scores) const;
    virtual ~OffspringGenerator() = default;
private:
    /// <summary>
    /// The builder strategy.
    /// </summary>
    std::unique_ptr<IndividualsBuilderStrategy> m_Builder;
    /// <summary>
    /// The preservation strategy.
    /// </summary>
    std::unique_ptr<PreservationStrategy> m_PreservationStrategy;
};
}
