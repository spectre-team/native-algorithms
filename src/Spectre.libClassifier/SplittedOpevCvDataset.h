/*
* SplittedOpenCvDataset.h
* Pair of OpenCvDatasets - training and test
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
#include "Spectre.libClassifier/OpenCvDataset.h"

namespace Spectre::libClassifier {
/// <summary>
/// Class used to create dataset having separated training and test datasets.
/// </summary>
class SplittedOpenCvDataset
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="SplittedOpenCvDataset"/> class.
    /// </summary>
    /// <param name="training">The training dataset.</param>
    /// <param name="test">The test dataset.</param>
    SplittedOpenCvDataset::SplittedOpenCvDataset(OpenCvDataset&& training, OpenCvDataset&& test);
    /// <summary>
    /// Initializes a new instance of the <see cref="SplittedOpenCvDataset"/> class.
    /// </summary>
    /// <param name="training">The training dataset.</param>
    /// <param name="test">The test dataset.</param>
    /// <param name="test">The indexes.</param>
    SplittedOpenCvDataset::SplittedOpenCvDataset(OpenCvDataset&& training, OpenCvDataset&& test, std::vector<int> indexes);
    /// <summary>
    /// Gets vector of bool with size of training + test size and with true values from parameter placed in right positions.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <returns>Dataset splitted into training and validation subsets.</returns>
    std::vector<bool> getWholeDatasetBinaryDataFromTrainingDatasetBinaryData(const std::vector<bool> trainingBinaryData) const;
    /// <summary>
    /// The training set.
    /// </summary>
    OpenCvDataset m_TrainingSet;
    /// <summary>
    /// The test set.
    /// </summary>
    OpenCvDataset m_TestSet;
    /// <summary>
    /// The shuffled indexes from which the data was split.
    /// </summary>
    std::vector<int> m_Indexes;
private:
};
}
