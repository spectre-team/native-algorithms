/*
* GaFitnessFunctionTest.cpp
* Tests GaFitnessFunction class.
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

#include <gtest/gtest.h>
#include "Spectre.libClassifier/Types.h"
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libGaClassifier/GaFitnessFunction.h"
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libException/InconsistentArgumentSizesException.h"

namespace
{
using namespace spectre::supervised;

TEST(GaFitnessFunctionTestInitialization, correct_ga_fitness_function_initialization)
{
    const std::vector<DataType> training_data{ 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f,
        0.4f, 1.6f, 0.9f, 1.2f, 2.2f, 0.7f, 1.3f, 2.0f, 1.4f, 0.7f, 0.7f, 0.9f };
    const std::vector<DataType> test_data{ 0.8f, 0.3f, 1.2f, 0.7f, 1.9f, 0.2f, 1.2f, 1.3f, 1.2f };
    const std::vector<Label> training_labels{ 1, 1, 0, 1, 0, 0, 1 };
    const std::vector<Label> test_labels{ 1, 0, 0 };
    OpenCvDataset trainingSet(std::move(training_data), std::move(training_labels));
    OpenCvDataset testSet(std::move(test_data), std::move(test_labels));
    SplittedOpenCvDataset data(std::move(trainingSet), std::move(testSet));
    Svm svm{};
    EXPECT_NO_THROW(GaFitnessFunction(svm, data));
}

class GaFitnessFunctionTest : public ::testing::Test
{
public:
    GaFitnessFunctionTest() :
        trainingSet(training_data, training_labels),
        testSet(test_data, test_labels),
        data(std::move(trainingSet), std::move(testSet)),
        gaFitnessFunction(svm, data)
    {};

protected:
    const std::vector<DataType> training_data{ 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f,
        0.4f, 1.6f, 0.9f, 1.2f, 2.2f, 0.7f, 1.3f, 2.0f, 1.4f, 0.7f, 0.7f, 0.9f };
    const std::vector<DataType> test_data{ 0.8f, 0.3f, 1.2f, 0.7f, 1.9f, 0.2f, 1.2f, 1.3f, 1.2f };
    const std::vector<Label> training_labels{ 1, 1, 0, 1, 0, 0, 1 };
    const std::vector<Label> test_labels{ 1, 0, 0 };
    OpenCvDataset trainingSet;
    OpenCvDataset testSet;
    SplittedOpenCvDataset data;
    Svm svm{};
    GaFitnessFunction gaFitnessFunction;
};

TEST_F(GaFitnessFunctionTest, svm_fit)
{
    spectre::algorithm::genetic::Individual individual(std::vector<bool> { true, false, true, true, false, false, true });
    EXPECT_NO_THROW(gaFitnessFunction(individual));
}

TEST_F(GaFitnessFunctionTest, throws_when_fitting_svm_on_inconsistent_size_individual)
{
    spectre::algorithm::genetic::Individual too_short_individual({ true, false, true, true, false, true });
    EXPECT_THROW(gaFitnessFunction(too_short_individual), spectre::core::exception::InconsistentArgumentSizesException);
}

}
