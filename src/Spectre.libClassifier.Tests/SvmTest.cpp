/*
* SvmTest.cpp
* Tests Svm classifier
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

#include <gtest/gtest.h>
#include "Spectre.libClassifier/Types.h"
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libClassifier/UntrainedClassifierException.h"
#include "Spectre.libClassifier/InsufficientDataException.h"

namespace
{
using namespace spectre::supervised;
using namespace spectre::supervised::exception;

class SvmTest : public ::testing::Test
{
protected:
    const std::vector<Label> m_Labels = { 1, 1, 0, 0, 1 };
    const std::vector<DataType> m_Features = {
        1., 1.,
        2., 2.,
        -1., -1.,
        -2., -2.,
        3., 3.
    };
    OpenCvDataset m_Data = std::move(OpenCvDataset(m_Features, m_Labels));
};

TEST_F(SvmTest, initializes)
{
    EXPECT_NO_THROW(Svm());
}

TEST_F(SvmTest, fits_with_no_exception)
{
    Svm svm;
    EXPECT_NO_THROW(svm.Fit(m_Data));
}

TEST_F(SvmTest, predicts_with_no_throw)
{
    Svm svm;
    svm.Fit(m_Data);
    EXPECT_NO_THROW(svm.Predict(m_Data));
}

TEST_F(SvmTest, predicts_sign)
{
    Svm svm;
    svm.Fit(m_Data);
    const auto prediction = svm.Predict(m_Data);
    for (auto i = 0u; i < m_Labels.size(); ++i)
    {
        EXPECT_EQ(m_Labels[i], prediction[i]);
    }
}

TEST_F(SvmTest, predicts_small_data)
{
    const std::vector<Label> tempLabels = { 1 };
    const std::vector<DataType> tempFeatures = {
        1., 1.
    };
    OpenCvDataset tempData(tempFeatures, tempLabels);
    Svm svm;
    EXPECT_THROW(svm.Fit(tempData), InsufficientDataException);
}

TEST_F(SvmTest, fit_with_non_binary_labels_throw_not_a_binary_exception)
{
    const std::vector<Label> tempLabels = { 2 };
    const std::vector<DataType> tempFeatures = {
        1., 1.
    };
    OpenCvDataset tempData(tempFeatures, tempLabels);
    Svm svm;
    EXPECT_THROW(svm.Fit(tempData), NotABinaryLabelException);
}

TEST_F(SvmTest, get_support_vector_number_test)
{
    Svm svm;
    svm.Fit(m_Data);
    EXPECT_EQ(svm.GetNumberOfSupportVectors(), 1);
}

TEST_F(SvmTest, predict_throws_without_train)
{
    Svm svm;
    EXPECT_THROW(svm.Predict(m_Data), UntrainedClassifierException);
}
}
