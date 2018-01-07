/*
* GenerationFactory.cpp
* Builds different versions of Generation.
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

#include "GenerationFactory.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"
#include <memory>
#include <algorithm>

namespace spectre::algorithm::genetic
{
GenerationFactory::GenerationFactory(size_t size, size_t individualSize, size_t initialFillup)
    : m_Size(size),
      m_IndividualSize(individualSize),
      m_InitialFillup(initialFillup)
{
    if (initialFillup > individualSize)
    {
        throw core::exception::ArgumentOutOfRangeException<size_t>("initialFillup", 0, individualSize, initialFillup);
    }
}

Generation GenerationFactory::buildDefaultGeneration(Seed seed) const
{
    std::vector<Individual> individuals;
    for (auto i = 0u; i < m_Size; i++)
    {
        individuals.push_back(buildDefaultIndividual(seed + i));
    }
    Generation generation(std::move(individuals));
    return generation;
}

Individual GenerationFactory::buildDefaultIndividual(Seed seed) const
{
    std::vector<bool> binaryData;
    for (auto i = 0u; i < m_InitialFillup; i++)
    {
        binaryData.push_back(true);
    }
    for (auto i = m_InitialFillup; i < m_IndividualSize; i++)
    {
        binaryData.push_back(false);
    }
    RandomNumberGenerator rng(seed);
    std::shuffle(binaryData.begin(), binaryData.end(), rng);
    Individual individual(std::move(binaryData));
    return individual;
}
}
