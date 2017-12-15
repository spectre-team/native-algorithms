/*
* ConfusionMatrixTest.cpp
* Tests ConfusionMatrix
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
#include "Spectre.libClassifier/ConfusionMatrix.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"
#include "Spectre.libClassifier/NotABinaryLabelException.h"

namespace
{
using namespace Spectre::libClassifier;

class ConfusionMatrixInitializationTest : public ::testing::Test
{
public:
    ConfusionMatrixInitializationTest() {};

protected:
    const unsigned int truePositives = 5;
    const unsigned int trueNegatives = 3;
    const unsigned int falsePositives = 4;
    const unsigned int falseNegatives = 6;
    const double expectedDice = 0.5;
    const std::vector<Label> actual{ 1, 0, 1, 1, 0 };
    const std::vector<Label> expected{ 1, 0, 1, 1, 0 };
    const std::vector<Label> incorrectActual{ 1, -1, 1, 1, 0 };
    const std::vector<Label> incorrectExpected{ 1, -1, 1, 1, 0 };
    const std::vector<Label> shorterExpected{ 1, 1, 0 };
};

TEST_F(ConfusionMatrixInitializationTest, confusion_matrix_initialization)
{
    EXPECT_NO_THROW(ConfusionMatrix(truePositives, trueNegatives, falsePositives, falseNegatives));
}

TEST_F(ConfusionMatrixInitializationTest, confusion_matrix_initialization_throws_on_incorrect_actual_and_expected_label_sizes)
{
    EXPECT_THROW(ConfusionMatrix(actual, shorterExpected), Spectre::libException::InconsistentArgumentSizesException);
}

TEST_F(ConfusionMatrixInitializationTest, confusion_matrix_initialization_throws_on_incorrect_actual_labels)
{
    EXPECT_THROW(ConfusionMatrix(incorrectActual, expected), NotABinaryLabelException);
}

TEST_F(ConfusionMatrixInitializationTest, confusion_matrix_initialization_throws_on_incorrect_expected_labels)
{
    EXPECT_THROW(ConfusionMatrix(actual, incorrectExpected), NotABinaryLabelException);
}

TEST_F(ConfusionMatrixInitializationTest, check_confusion_matrix_values)
{
    ConfusionMatrix confusionMatrix(truePositives, trueNegatives, falsePositives, falseNegatives);
    EXPECT_EQ(confusionMatrix.TruePositive, truePositives);
    EXPECT_EQ(confusionMatrix.TrueNegative, trueNegatives);
    EXPECT_EQ(confusionMatrix.FalsePositive, falsePositives);
    EXPECT_EQ(confusionMatrix.FalseNegative, falseNegatives);
    EXPECT_EQ(confusionMatrix.DiceIndex, expectedDice);
}

}
