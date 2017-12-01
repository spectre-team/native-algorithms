/*
* Limit.h
* Limit content of the iterable.
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
#include <vector>
#include "Spectre.libException/OutOfRangeException.h"

namespace Spectre::libPlatform::Functional
{
    /// <summary>
    /// Filter the specified collection.
    /// </summary>
    /// <param name="collection">The collection.</param>
    /// <param name="limit">The limit.</param>
    /// <returns>Vector limited to "limit" values</returns>
    template <typename DataType>
    std::vector<DataType> limit(gsl::span<const DataType> collection, int limit)
    {
        if (limit > collection.size())
        {
            throw libException::OutOfRangeException(limit, collection.size());
        }
        std::vector<DataType> result{};
        result.reserve(limit);
        for (auto i = 0; i < limit; i++)
        {
            result.push_back(collection[i]);
        }
        return result;
    }
}
