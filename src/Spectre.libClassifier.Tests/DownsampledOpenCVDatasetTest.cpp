/*
* DownsampledOpenCVDatasetTest.cpp
* Tests DownsampledOpenCVDataset
*
Copyright 2017 Wojciech Wilgierz

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

#include <gtest/gtest.h>
#include "Spectre.libClassifier/Types.h"
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libClassifier/DownsampledOpenCVDataset.h"

namespace
{
using namespace Spectre::libClassifier;

class DownscaledOpenCVDatasetInitializationTest : public ::testing::Test
{
public:
    DownscaledOpenCVDatasetInitializationTest():
        dataset(data, labels) {};

protected:
    const std::vector<DataType> data{ 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f, 0.3f };
    const std::vector<Label> labels{ 0, 1, 0, 0, 0, 0, 1, 1, 0, 1 };
    const unsigned maximumSubsetSizeGreater = 5;
    const unsigned maximumSubsetSizeLower = 2;
    const double trainingRate = 0.5;
    const double trainingRateWithMoreTraining = 0.75;
    OpenCvDataset dataset;

    void SetUp() override
    {
    }
};

TEST_F(DownscaledOpenCVDatasetInitializationTest, returns_dataset_of_expected_size_with_greater_maximum_subset_size)
{
    DownsampledOpenCVDataset downscaledOpenCvDataset(std::move(dataset), maximumSubsetSizeGreater, trainingRate);
    SplittedOpenCvDataset splittedOpenCvDataset = downscaledOpenCvDataset.getLimitedDownSampledOpenCVDataset();
    EXPECT_EQ(splittedOpenCvDataset.m_TrainingSet.size(), 4);
    EXPECT_EQ(splittedOpenCvDataset.m_TestSet.size(), 4);
}

TEST_F(DownscaledOpenCVDatasetInitializationTest, returns_dataset_of_expected_size_with_greater_maximum_subset_size_with_more_training)
{
    DownsampledOpenCVDataset downscaledOpenCvDataset(std::move(dataset), maximumSubsetSizeGreater, trainingRateWithMoreTraining);
    SplittedOpenCvDataset splittedOpenCvDataset = downscaledOpenCvDataset.getLimitedDownSampledOpenCVDataset();
    EXPECT_EQ(splittedOpenCvDataset.m_TrainingSet.size(), 6);
    EXPECT_EQ(splittedOpenCvDataset.m_TestSet.size(), 2);
}

TEST_F(DownscaledOpenCVDatasetInitializationTest, returns_dataset_of_expected_size_with_lower_maximum_subset_size)
{
    DownsampledOpenCVDataset downscaledOpenCvDataset(std::move(dataset), maximumSubsetSizeLower, trainingRate);
    SplittedOpenCvDataset splittedOpenCvDataset = downscaledOpenCvDataset.getLimitedDownSampledOpenCVDataset();
    EXPECT_EQ(splittedOpenCvDataset.m_TrainingSet.size(), 2);
    EXPECT_EQ(splittedOpenCvDataset.m_TestSet.size(), 2);
}

}
