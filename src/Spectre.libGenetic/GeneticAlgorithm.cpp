/*
* GeneticAlgorithm.cpp
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

#include "Spectre.libException/NullPointerException.h"
#include "Generation.h"
#include "GeneticAlgorithm.h"

namespace spectre::algorithm::genetic
{
GeneticAlgorithm::GeneticAlgorithm(std::unique_ptr<OffspringGenerator> offspringGenerator, std::unique_ptr<Scorer> scorer, std::unique_ptr<StopCondition> stopCondition,
                                   std::unique_ptr<BaseIndividualFeasibilityCondition> individualFeasibilityConditions)
    : m_OffspringGenerator(std::move(offspringGenerator)),
      m_Scorer(std::move(scorer)),
      m_StopCondition(std::move(stopCondition)),
      m_IndividualFeasibilityConditions(std::move(individualFeasibilityConditions))
{
    if (m_OffspringGenerator == nullptr)
    {
        throw spectre::core::exception::NullPointerException("offspringGenerator");
    }
    if (m_Scorer == nullptr)
    {
        throw spectre::core::exception::NullPointerException("scorer");
    }
    if (m_StopCondition == nullptr)
    {
        throw spectre::core::exception::NullPointerException("stopCondition");
    }
}

Generation GeneticAlgorithm::evolve(Generation &&generation) const
{
    while (!m_StopCondition->operator()())
    {
        const auto scores = m_Scorer->Score(generation);
        generation = m_OffspringGenerator->next(generation, scores);
    }
    return generation;
}
}
