/*
* OpenCvDatasetTest.cpp
* Tests OpenCvDataset
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
#include <memory>
#include <span.h>
#include <opencv2/core/mat.hpp>
#include "Spectre.libException/InconsistentArgumentSizesException.h"
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libClassifier/EmptyOpenCvDatasetException.h"
#include <gmock/gmock-matchers.h>
#include "Spectre.libClassifier/DatasetFilter.h"

namespace
{
using namespace spectre::supervised;
using namespace spectre::core::exception;
using namespace spectre::supervised::exception;

class OpenCvDatasetInitializationTest : public ::testing::Test
{
protected:
    const std::vector<DataType> data_empty {};
    const std::vector<DataType> data_long { 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f };
    const std::vector<DataType> data_short { 0.5f, 0.4f, 0.6f };
    const std::vector<Label> labels_empty {};
    const std::vector<Label> labels { 3, 7, 14 };
    const std::vector<Label> labels_short { 2 };
    const std::vector<Label> labels_too_long { 3, 7, 14, 5 };
};

TEST_F(OpenCvDatasetInitializationTest, correct_dataset_opencv_initialization_col_size_one)
{
    EXPECT_NO_THROW(OpenCvDataset(data_short, labels));
}

TEST_F(OpenCvDatasetInitializationTest, correct_dataset_opencv_initialization)
{
    EXPECT_NO_THROW(OpenCvDataset(data_long, labels));
}

TEST_F(OpenCvDatasetInitializationTest, throws_for_inconsistent_size)
{
    EXPECT_THROW(OpenCvDataset(data_short, labels_too_long), InconsistentArgumentSizesException);
}

TEST_F(OpenCvDatasetInitializationTest, throws_for_empty_data)
{
    EXPECT_THROW(OpenCvDataset(data_empty, labels_empty), EmptyOpenCvDatasetException);
}

TEST_F(OpenCvDatasetInitializationTest, correct_dataset_opencv_initialization_from_mat_col_size_one)
{
    std::vector<DataType> tmp_data(data_short);
    std::vector<Label> tmp_labels(labels);
    cv::Mat mat_data(3, 1, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(3, 1, CV_LABEL_TYPE, tmp_labels.data());
    EXPECT_NO_THROW(OpenCvDataset(mat_data, mat_labels));
}

TEST_F(OpenCvDatasetInitializationTest, correct_dataset_opencv_initialization_from_mat)
{
    std::vector<DataType> tmp_data(data_long);
    std::vector<Label> tmp_labels(labels);
    cv::Mat mat_data(3, 3, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(3, 1, CV_LABEL_TYPE, tmp_labels.data());
    EXPECT_NO_THROW(OpenCvDataset(mat_data, mat_labels));
}

TEST_F(OpenCvDatasetInitializationTest, throws_for_inconsistent_size_from_mat)
{
    std::vector<DataType> tmp_data(data_long);
    std::vector<Label> tmp_labels(labels_too_long);
    cv::Mat mat_data(3, 3, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(4, 1, CV_LABEL_TYPE, tmp_labels.data());
    EXPECT_THROW(OpenCvDataset(mat_data, mat_labels), InconsistentArgumentSizesException);
}

TEST_F(OpenCvDatasetInitializationTest, throws_for_empty_data_from_mat)
{
    std::vector<DataType> tmp_data(data_empty);
    std::vector<Label> tmp_labels(labels_empty);
    cv::Mat mat_data(0, 0, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(0, 0, CV_LABEL_TYPE, tmp_labels.data());
    EXPECT_THROW(OpenCvDataset(mat_data, mat_labels), EmptyOpenCvDatasetException);
}

TEST_F(OpenCvDatasetInitializationTest, move_invalidates_old_instance)
{
    std::vector<DataType> tmp_data(data_short);
    std::vector<Label> tmp_labels(labels);
    cv::Mat mat_data(3, 1, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(3, 1, CV_LABEL_TYPE, tmp_labels.data());
    OpenCvDataset oldDataset(mat_data, mat_labels);
    OpenCvDataset newDataset(std::move(oldDataset));
    EXPECT_EQ(0u, oldDataset.size());
    EXPECT_EQ(0u, oldDataset.GetData().size());
    EXPECT_EQ(0u, oldDataset.GetSampleMetadata().size());
    EXPECT_THROW(oldDataset[0], OutOfRangeException);
    EXPECT_THROW(oldDataset.GetSampleMetadata(0), OutOfRangeException);
    EXPECT_EQ(0, oldDataset.getMatData().size().area());
    EXPECT_EQ(0, oldDataset.getMatLabels().size().area());
}

TEST_F(OpenCvDatasetInitializationTest, create_opencvdataset_from_two_opencvdatasets)
{
    std::vector<DataType> tmp_data(data_short);
    std::vector<Label> tmp_labels(labels_short);
    std::vector<DataType> tmp_data2(data_long);
    std::vector<Label> tmp_labels2(labels);
    cv::Mat mat_data(3, 1, CV_TYPE, tmp_data.data());
    cv::Mat mat_labels(3, 1, CV_LABEL_TYPE, tmp_labels.data());
    cv::Mat mat_data2(3, 1, CV_TYPE, tmp_data2.data());
    cv::Mat mat_labels2(3, 1, CV_LABEL_TYPE, tmp_labels2.data());
    OpenCvDataset firstDataset(mat_data, mat_labels);
    OpenCvDataset secondDataset(mat_data2, mat_labels2);
    size_t size = firstDataset.size() + secondDataset.size();
    size_t getDataSize = firstDataset.GetData().size() + secondDataset.GetData().size();
    size_t getSampleDataSize = firstDataset.GetSampleMetadata().size() + secondDataset.GetSampleMetadata().size();
    size_t getMatDataSize = firstDataset.getMatData().size().area() + secondDataset.getMatData().size().area();
    size_t getMatLabelsSize = firstDataset.getMatLabels().size().area() + secondDataset.getMatLabels().size().area();
    OpenCvDataset newDataset(std::move(firstDataset), std::move(secondDataset));
    EXPECT_EQ(newDataset.size(), size);
    EXPECT_EQ(newDataset.GetData().size(), getDataSize);
    EXPECT_EQ(newDataset.GetSampleMetadata().size(), getSampleDataSize);
    EXPECT_EQ(newDataset.getMatData().size().area(), getMatDataSize);
    EXPECT_EQ(newDataset.getMatLabels().size().area(), getMatLabelsSize);
}

class OpenCvDatasetTest : public ::testing::Test
{
public:
    OpenCvDatasetTest() {}

    static bool compareCVMats(cv::Mat mat1, cv::Mat mat2)
    {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols)
            return false;
        for (auto i = 0; i < (mat1.rows * mat1.cols); i++)
        {
            if (mat1.data[i] != mat2.data[i])
                return false;
        }
        return true;
    }

protected:
    const std::vector<DataType> data { 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f };
    const std::vector<Label> labels { 3, 7, 14 };
    const std::vector<DataType> result_data{ 0.5f, 0.4f, 0.6f, 2.1f, 1.0f, 0.6f };
    const std::vector<Label> result_labels{ 3, 14 };
    std::vector<bool> include{ true, false, true };
    const size_t rowSize = data.size() / labels.size();
    std::unique_ptr<OpenCvDataset> dataset;

    void SetUp() override
    {
        dataset = std::make_unique<OpenCvDataset>(data, labels);
    }
};

TEST_F(OpenCvDatasetTest, throws_on_access_to_data_out_of_range)
{
    EXPECT_THROW((*dataset)[4], OutOfRangeException);
}

TEST_F(OpenCvDatasetTest, check_getting_in_range_data)
{
    auto test = (*dataset)[1];
    std::vector<DataType> check({ 1.1f, 1.6f, 0.7f });
    for (auto i = 0u; i < check.size(); i++)
    {
        EXPECT_EQ(test[i], check[i]);
    }
}

TEST_F(OpenCvDatasetTest, throws_on_access_to_label_out_of_range)
{
    EXPECT_THROW(dataset->GetSampleMetadata(4), OutOfRangeException);
}

TEST_F(OpenCvDatasetTest, check_getting_in_range_label)
{
    auto test = dataset->GetSampleMetadata(1);
    Label check = 7;
    EXPECT_EQ(test, check);
}

TEST_F(OpenCvDatasetTest, get_dataset_metadata)
{
    const auto &check = spectre::core::dataset::Empty::instance();
    EXPECT_EQ(&dataset->GetDatasetMetadata(), &check);
}

TEST_F(OpenCvDatasetTest, get_data)
{
    const auto &local_dataset = *dataset;
    gsl::span<const Observation> result = dataset->GetData();
    const auto numberOfObservations = local_dataset.size();
    const auto numberofColumns = local_dataset[0].size();

    ASSERT_EQ(result.size(), numberOfObservations);
    for (auto i = 0u; i < numberOfObservations; i++)
    {
        EXPECT_EQ(result[i].size(), numberofColumns) << i;
        for (auto j = 0u; j < static_cast<size_t>(numberofColumns); ++j)
        {
            EXPECT_EQ(result[i][j], local_dataset[i][j]) << i << " " << j;
        }
    }
}

TEST_F(OpenCvDatasetTest, get_whole_labels)
{
    gsl::span<const Label> result = dataset->GetSampleMetadata();
    EXPECT_EQ(result.size(), labels.size());
    for (auto i = 0u; i < labels.size(); i++)
    {
        EXPECT_EQ(result[i], labels[i]);
    }
}

TEST_F(OpenCvDatasetTest, get_some_labels)
{
    for (auto i = 0u; i < labels.size(); i++)
    {
        EXPECT_EQ(labels[i], dataset->GetSampleMetadata(i));
    }
}

TEST_F(OpenCvDatasetTest, check_correct_dataset_size)
{
    EXPECT_EQ(dataset->size(), labels.size());
}

TEST_F(OpenCvDatasetTest, check_getting_data_to_mat)
{
    std::vector<DataType> mat_data { 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f };
    cv::Mat check(3, 3, CV_TYPE, mat_data.data());
    cv::Mat result = dataset->getMatData();
    EXPECT_TRUE(compareCVMats(check, result));
}

TEST_F(OpenCvDatasetTest, check_getting_labels_to_mat)
{
    std::vector<Label> mat_labels { 3, 7, 14 };
    cv::Mat check(3, 1, CV_LABEL_TYPE, mat_labels.data());
    cv::Mat result = dataset->getMatLabels();
    EXPECT_TRUE(compareCVMats(check, result));
}

TEST_F(OpenCvDatasetTest, check_if_creating_copy)
{
    std::unique_ptr<OpenCvDataset> result;
    const std::vector<DataType> tmpData { 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f };
    const std::vector<Label> tmpLabels { 3, 7, 14 };
    const auto numberOfObservations = tmpLabels.size();
    const auto numberOfColumns = tmpData.size() / numberOfObservations;

    // @gmrukwa: Opening new scope, so variables will get destroyed.
    {
        const std::vector<DataType> tmpDataDestroyed(tmpData);
        const std::vector<Label> tmpLabelsDestroyed(tmpLabels);

        result = std::make_unique<OpenCvDataset>(tmpDataDestroyed, tmpLabelsDestroyed);
    }

    ASSERT_EQ(result->size(), numberOfObservations);

    const auto &result_dataset = *result;
    for (auto i = 0u; i < numberOfObservations; ++i)
    {
        for (auto j = 0u; j < static_cast<size_t>(result_dataset[i].size()); ++j)
        {
            EXPECT_EQ(result_dataset[i][j], tmpData[i * numberOfColumns + j]) << i << " " << j;
        }
    }
}

TEST_F(OpenCvDatasetTest, returns_observation_with_valid_size_through_square_braces)
{
    const auto &observation = dataset->operator[](0);
    ASSERT_EQ(rowSize, observation.size());
}

TEST_F(OpenCvDatasetTest, returns_observation_with_valid_entries_through_square_braces)
{
    const auto &observation = dataset->operator[](0);
    for (auto i = 0u; i < rowSize; ++i)
    {
        EXPECT_EQ(data[i], observation[i]);
    }
}

TEST_F(OpenCvDatasetTest, returns_correct_filtered_data)
{
    OpenCvDataset filtered = getFilteredOpenCvDataset(*dataset.get(), include);
    gsl::span<const Observation> filtered_data_gsl = filtered.GetData();
    std::vector<DataType> filtered_data{};
    for (auto observation: filtered_data_gsl)
    {
        for (auto dataInObservation: observation)
        {
            filtered_data.push_back(dataInObservation);
        }
    }
    gsl::span<const Label> filtered_labels_gsl = filtered.GetSampleMetadata();
    std::vector<Label> filtered_labels(filtered_labels_gsl.begin(), filtered_labels_gsl.end());
    for (auto i = 0u; i < result_data.size(); i++)
    {
        EXPECT_EQ(result_data[i], filtered_data[i]);
    }
    for (auto i = 0u; i < result_labels.size(); i++)
    {
        EXPECT_EQ(result_labels[i], filtered_labels[i]);
    }
}
}
