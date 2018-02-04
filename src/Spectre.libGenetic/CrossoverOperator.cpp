/*
* CrossoverOperator.cpp
* Generates new Individual from two parents.
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

#include "CrossoverOperator.h"
#include "InconsistentChromosomeLengthException.h"
#include "InconsistentMinimalAndMaximalFillupException.h"
#include "Individual.h"

namespace spectre::algorithm::genetic
{
CrossoverOperator::CrossoverOperator(Seed rngSeed, size_t minimalFillup, size_t maximalFillup, BaseIndividualFeasibilityCondition* individualFeasibilityCondition):
    m_RandomNumberGenerator(rngSeed),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup),
    m_IndividualFeasibilityCondition(individualFeasibilityCondition)
{
    if (m_MinimalFillup > m_MaximalFillup)
    {
        throw InconsistentMinimalAndMaximalFillupException(m_MinimalFillup, m_MaximalFillup);
    }
}

Individual CrossoverOperator::operator()(const Individual &first, const Individual &second)
{
    if (first.size() != second.size())
    {
        throw InconsistentChromosomeLengthException(first.size(), second.size());
    }
    Individual child(first);
    do
    {
         child = cross(first, second);
    } while (m_IndividualFeasibilityCondition != nullptr && !m_IndividualFeasibilityCondition->checkCondition(child));
    return child;
}

Individual CrossoverOperator::cross(const Individual& first, const Individual& second)
{
    std::uniform_int_distribution<size_t> distribution(0, first.size());
    const auto cuttingPoint = distribution(m_RandomNumberGenerator);
    const auto endOfFirst = first.begin() + cuttingPoint;
    const auto beginningOfSecond = second.begin() + cuttingPoint;
    std::vector<bool> phenotype;
    phenotype.reserve(first.size());
    phenotype.insert(phenotype.end(), first.begin(), endOfFirst);
    phenotype.insert(phenotype.end(), beginningOfSecond, second.end());

    auto fillup = 0u;
    for (auto bit : phenotype)
    {
        fillup += bit;
    }

    if (fillup >= m_MinimalFillup && fillup <= m_MaximalFillup)
    {
        return Individual(std::move(phenotype));
    }
    else
    {
        return first;
    }
}

}
