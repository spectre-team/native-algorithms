/*
* DatasetFactory.h
* Creates OpenCvDataset from external sources.
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
#include "OpenCvDataset.h"

namespace spectre::supervised
{
/// <summary>
/// Creates OpenCvDataset from external sources.
/// </summary>
class DatasetFactory
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="DatasetFactory"/> class.
    /// </summary>
    DatasetFactory();
    /// <summary>
    /// Creates new OpenCvDataset from file.
    /// </summary>
    /// <param name="filename">The filename.</param>
    OpenCvDataset create(std::string filename) const;
};
}
