/*
* Individual.h
* Binary individual class.
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
#include <vector>
#include "Spectre.libGenetic/DataTypes.h"

namespace spectre::algorithm::genetic
{
/// <summary>
/// Binary representation of an individual chromosome in genetic algorithm.
/// </summary>
class Individual : public std::vector<bool>
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Individual"/> class.
    /// </summary>
    /// <param name="binaryData">The binary data.</param>
    explicit Individual(std::vector<bool> &&binaryData);
    /// <summary>
    /// Compares object with other
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>True, if binary data inside is the same.</returns>
    bool operator==(const Individual &other) const;
    /// <summary>
    /// Compares object with other
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>False, if binary data inside is the same.</returns>
    bool operator!=(const Individual &other) const;
    /// <summary>
    /// Overwrites individual with another object data.
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>This instance.</returns>
    Individual& operator=(const Individual &other);
};
}
