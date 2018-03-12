/*
* DatasetFilter.h
* Class consisting functions for filtering datasets.
*
Copyright 2018 Spectre Team

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
#include "Spectre.libFunctional/Filter.h"
#include "OpenCvDataset.h"

namespace spectre::supervised
{
/// <summary>
/// Returns OpenCvDataset filtered by vector of bool which tells which parts to leave.
/// </summary>
/// <param name="dataset">The dataset.</param>
/// <param name="filterPattern">The vector of bool consisting information which parts of dataset to leave.</param>
/// <returns>Filtered dataset.</returns>
inline OpenCvDataset getFilteredOpenCvDataset(const OpenCvDataset& dataset, const std::vector<bool>& filterPattern)
{
    const auto& dataToFilter = dataset.GetData();
    const auto twoDimensionalFilteredData = core::functional::filter(dataToFilter, filterPattern);
    std::vector<DataType> oneDimensionalFilteredData;
    oneDimensionalFilteredData.reserve(twoDimensionalFilteredData.size() * dataset[0].size());
    for (const auto& observation : twoDimensionalFilteredData)
    {
        oneDimensionalFilteredData.insert(oneDimensionalFilteredData.end(), observation.begin(), observation.end());
    }
    const auto filteredLabels = core::functional::filter(dataset.GetSampleMetadata(), filterPattern);
    OpenCvDataset individualDataset(oneDimensionalFilteredData, filteredLabels);
    return individualDataset;
}
}
