/*
* RandomMultipleValidationSelectionScenarioTest.cpp
* Tests RandomMultipleValidationSelectionScenario class
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

#include "Spectre.libGenetic/DataTypes.h"
#include <gtest/gtest.h>
#include "Spectre.GaSvmNative/RandomMultipleValidationSelectionScenario.h"

namespace
{
using namespace Spectre::libGenetic;
using namespace Spectre::GaSvmNative;

const double TRAINING_SET_SPLIT_RATE = 0.7;
const double TRAINING_DOWNSAMPLE_RATE = 0.5;
const size_t MAXIMUM_SUBSET_SIZE = 6;
const double MUTATION_RATE = 0.5;
const double BIT_SWAP_RATE = 0.5;
const double PRESERVATION_RATE = 0.5;
const unsigned int GENERATIONS_NUMBER = 5u;
const unsigned int POPULATION_SIZE = 20u;
const unsigned int INITIAL_FILLUP = 3u;
const unsigned int NUMBER_OF_RESTARTS = 2u;
const Seed SEED = 1;

TEST(RandomMultipleValidationSelectionScenarioInitialization, initializes)
{
    EXPECT_NO_THROW(RandomMultipleValidationSelectionScenario(TRAINING_SET_SPLIT_RATE,
        TRAINING_DOWNSAMPLE_RATE,
        MAXIMUM_SUBSET_SIZE,
        MUTATION_RATE,
        BIT_SWAP_RATE,
        PRESERVATION_RATE,
        GENERATIONS_NUMBER,
        POPULATION_SIZE,
        INITIAL_FILLUP,
        "initialize_test",
        NUMBER_OF_RESTARTS));
}

class RandomMultipleValidationSelectionScenarioInitializationTest : public ::testing::Test
{
public:
    RandomMultipleValidationSelectionScenarioInitializationTest() :
        dataSet(std::make_unique<Spectre::libClassifier::OpenCvDataset>(data, labels)) {}

protected:
    const std::vector<Spectre::libClassifier::DataType> data{
        0.5f, 0.4f, 0.6f,
        1.1f, 1.6f, 0.7f,
        2.1f, 1.0f, 0.6f,
        0.4f, 1.6f, 0.9f,
        1.2f, 2.2f, 0.7f,
        1.3f, 2.0f, 1.4f,
        0.7f, 0.7f, 0.9f,
        0.8f, 0.3f, 1.2f,
        0.7f, 1.9f, 0.2f,
        1.2f, 1.3f, 1.2f,
        0.5f, 0.2f, 1.1f,
        0.9f, 1.9f, 0.3f,
        0.7f, 0.9f, 1.0f,
        1.2f, 1.2f, 1.5f,
        1.8f, 0.4f, 0.2f,
        0.7f, 1.7f, 1.1f,
        1.2f, 1.3f, 1.4f,
        1.7f, 0.9f, 0.6f,
        1.2f, 1.3f, 1.2f,
        0.1f, 1.6f, 0.9f,
    };
    const std::vector<Spectre::libClassifier::Label> labels{ 1, 1, 0, 1, 0, 0, 0, 1, 0, 0,
                                                             0, 0, 0, 1, 1, 0, 0, 1, 1, 0 };
    std::unique_ptr<Spectre::libClassifier::OpenCvDataset> dataSet;

    void SetUp() override
    {
        dataSet = std::make_unique<Spectre::libClassifier::OpenCvDataset>(data, labels);
    }
};

TEST_F(RandomMultipleValidationSelectionScenarioInitializationTest, few_data_scenario)
{
    RandomMultipleValidationSelectionScenario scenario(TRAINING_SET_SPLIT_RATE,
        TRAINING_DOWNSAMPLE_RATE,
        MAXIMUM_SUBSET_SIZE,
        MUTATION_RATE,
        BIT_SWAP_RATE,
        PRESERVATION_RATE,
        GENERATIONS_NUMBER,
        POPULATION_SIZE,
        INITIAL_FILLUP,
        "random_multiple_validation_test_scenario",
        NUMBER_OF_RESTARTS,
        SEED);
    EXPECT_NO_THROW(scenario.execute(*dataSet));
}

}
