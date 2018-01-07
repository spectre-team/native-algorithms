/*
* GenerationFactory.h
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

#pragma once
#include "Spectre.libGenetic/DataTypes.h"
#include "Generation.h"
#include <memory>

namespace spectre::algorithm::genetic
{
/// <summary>
/// Factory for creating Generation objects with parameters.
/// </summary>
class GenerationFactory
{
public:
    /// <summary>
    /// Creates generation from parameters.
    /// </summary>
    /// <param name="size">Size of generation.</param>
    /// <param name="individualSize">Size of every individual in generation.</param>
    /// <param name="initialFillup">The initial fillup.</param>
    /// <param name="seed">The seed.</param>
    GenerationFactory(size_t size, size_t individualSize, size_t initialFillup);
    /// <summary>
    /// Creates generation with default parameter values.
    /// </summary>
    /// <returns>Created generation.</returns>
    Generation buildDefaultGeneration(Seed seed) const;
private:
    /// <summary>
    /// Creates individual with default parameter values.
    /// </summary>
    /// <returns>Created individual.</returns>
    Individual buildDefaultIndividual(Seed seed) const;
    /// <summary>
    /// The size of generation.
    /// </summary>
    size_t m_Size;
    /// <summary>
    /// The size of each individual.
    /// </summary>
    size_t m_IndividualSize;
    /// <summary>
    /// The number of true values in each individual.
    /// </summary>
    size_t m_InitialFillup;
};

}
