/*
* DownsampledOpenCVDataset.h
* Structure having same amount of cancer and noncancer cells for fitness function random multiple validation
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
#include <memory>
#include "OpenCvDataset.h"
#include "SplittedOpevCvDataset.h"

namespace spectre::supervised
{
/// <summary>
/// Class used to create dataset having the same amount of cancer and noncancer cells.
/// </summary>
class DownsampledOpenCVDataset
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="DownscaledOpenCVDataset"/> class.
    /// </summary>
    /// <param name="data">The dataset.</param>
    /// <param name="maximumSubsetSize">The maximum subset size.</param>
    DownsampledOpenCVDataset::DownsampledOpenCVDataset(OpenCvDataset data, size_t maximumSubsetSize, double trainingRate);
    /// <summary>
    /// Gets Splitted training and test dataset having the same amount cancer and noncancer cells.
    /// </summary>
    /// <param name="seed">The seed.</param>
    /// <returns>SplittedOpenCvDataset</returns>
    SplittedOpenCvDataset DownsampledOpenCVDataset::getLimitedDownSampledOpenCVDataset(Seed seed = 0) const;
private:
    /// <summary>
    /// The cancer cells.
    /// </summary>
    std::unique_ptr<OpenCvDataset> m_CancerCells;
    /// <summary>
    /// The noncancer cells.
    /// </summary>
    std::unique_ptr<OpenCvDataset> m_NonCancerCells;
    /// <summary>
    /// The maximum subset size.
    /// </summary>
    size_t m_MaximumSubsetSize;
    /// <summary>
    /// The training rate.
    /// </summary>
    double m_TrainingRate;
    /// <summary>
    /// Creates individual data with m_MaximumSubsetSize true values.
    /// </summary>
    /// <param name="datasetSize">The dataset size.</param>
    /// <returns>vector of bool</returns>
    std::vector<bool> DownsampledOpenCVDataset::getBinaryDataWithGivenTrueValueAmount(size_t datasetSize, Seed seed) const;
};
}