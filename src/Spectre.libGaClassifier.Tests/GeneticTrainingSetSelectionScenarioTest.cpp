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

#include <string>
#include "Spectre.libGenetic/DataTypes.h"
#include "Spectre.libGaClassifier/GeneticTrainingSetSelectionScenario.h"
#include <gtest/gtest.h>

namespace
{
using namespace spectre::supervised;

const std::string CLASSIFIER = "svm";
const double TRAINING_SET_SPLIT_RATE = 0.7;
const double MUTATION_RATE = 0.5;
const double BIT_SWAP_RATE = 0.5;
const double PRESERVATION_RATE = 0.5;
const unsigned int GENERATIONS_NUMBER = 30u;
const unsigned int POPULATION_SIZE = 20u;
const unsigned int INITIAL_FILLUP = 600u;
const unsigned int ITERATION_NUMBER = 1u;
const spectre::algorithm::genetic::Seed SEED = 454646;

TEST(GeneticTrainingSetSelectionScenarioInitialization, initializes)
{
    EXPECT_NO_THROW(GeneticTrainingSetSelectionScenario(CLASSIFIER, TRAINING_SET_SPLIT_RATE, MUTATION_RATE, BIT_SWAP_RATE,
        PRESERVATION_RATE, GENERATIONS_NUMBER, POPULATION_SIZE, INITIAL_FILLUP, ITERATION_NUMBER, SEED));
}

class GeneticTrainingSetSelectionScenarioTest : public ::testing::Test
{
public:
    GeneticTrainingSetSelectionScenarioTest()
        : scenario(CLASSIFIER, TRAINING_SET_SPLIT_RATE, MUTATION_RATE, BIT_SWAP_RATE, PRESERVATION_RATE,
            GENERATIONS_NUMBER, POPULATION_SIZE, INITIAL_FILLUP, ITERATION_NUMBER, SEED) {}
protected:
    GeneticTrainingSetSelectionScenario scenario;
};

TEST_F(GeneticTrainingSetSelectionScenarioTest, executeScenario)
{
    EXPECT_NO_THROW(scenario.execute("test_serious.txt", "test_serious_report", POPULATION_SIZE));
}

}