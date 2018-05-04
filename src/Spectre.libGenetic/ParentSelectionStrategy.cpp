/*
* ParentSelectionStrategy.cpp
* Strategy describing how parents are chosen to create offspring.
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

#include <algorithm>
#include "Spectre.libGenetic/ParentSelectionStrategy.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"
#include "InconsistentGenerationAndScoresLengthException.h"

namespace spectre::algorithm::genetic
{
ParentSelectionStrategy::ParentSelectionStrategy(Seed seed):
    m_RandomNumberGenerator(seed) { }

reference_pair<Individual> ParentSelectionStrategy::next(Generation &generation, gsl::span<const ScoreType> scores)
{
    if (generation.size() != static_cast<size_t>(scores.size()))
    {
        throw InconsistentGenerationAndScoresLengthException(generation.size(), scores.size());
    }
    const auto minWeight = *std::min_element(std::begin(scores), std::end(scores));
    const auto maxWeight = *std::max_element(std::begin(scores), std::end(scores));
    if (minWeight < 0)
    {
        throw spectre::core::exception::ArgumentOutOfRangeException<ScoreType>("scores", 0, std::numeric_limits<ScoreType>::max(), minWeight);
    }
    std::vector<ScoreType> defaultScores(scores.size(), 1);
    if (maxWeight <= 0)
    {
        scores = defaultScores;
    }
    std::discrete_distribution<size_t> indexDistribution(scores.begin(), scores.end());
    const auto first = indexDistribution(m_RandomNumberGenerator);
    const auto second = indexDistribution(m_RandomNumberGenerator);
    std::reference_wrapper<Individual> firstWrapped(generation[first]);
    std::reference_wrapper<Individual> secondWrapped(generation[second]);
    return std::make_pair(firstWrapped, secondWrapped);
}
}
