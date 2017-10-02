/*
* ObservationExtractor.h
* class for getting data from Individual object
*
Copyright 2017 Spectre Team

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
#include "Spectre.libDataset/IReadOnlyDataset.h"
#include "Spectre.libDataset/Empty.h"
#include "Spectre.libClassifier/OpenCvDataset.h"

namespace spectre::supervised {

using DataPointer = const spectre::core::dataset::IReadOnlyDataset<Observation, Label, spectre::core::dataset::Empty>*;

/// <summary>
/// Class used for getting dataset from individual.
/// </summary>
class ObservationExtractor
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ObservationExtractor"/> class.
    /// </summary>
    /// <param name="data">The data.</param>
    explicit ObservationExtractor(const DataPointer data);
    /// <summary>
    /// Gets data.
    /// </summary>
    /// <param name="individual">The individual.</param>
    /// <returns>OpenCvDataset</returns>
    OpenCvDataset getOpenCvDatasetFromIndividual(const std::vector<bool>& individual) const;

private:
    /// <summary>
    /// The dataset.
    /// </summary>
    const DataPointer m_Data;
};

}
