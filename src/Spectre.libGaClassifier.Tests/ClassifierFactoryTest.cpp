/*
* ClassifierFactoryTest.cpp
* The factory for creating classifiers
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

#include "Spectre.libGenetic/DataTypes.h"
#include <gtest/gtest.h>
#include "Spectre.libGaClassifier/ClassifierFactory.h"

namespace spectre::supervised
{
const std::string CLASSIFIER = "svm";
const double TRAINING_SET_SPLIT_RATE = 0.7;
const double MUTATION_RATE = 0.5;
const double BIT_SWAP_RATE = 0.5;
const double PRESERVATION_RATE = 0.5;
const unsigned int GENERATIONS_NUMBER = 5u;
const unsigned int POPULATION_SIZE = 5u;
const unsigned int INITIAL_FILLUP = 3u;
const algorithm::genetic::Seed SEED = 1;

TEST(ClassifierFactoryInitialization, initializes)
{
    EXPECT_NO_THROW(ClassifierFactory{});
}

class ClassifierFactoryTest : public ::testing::Test
{
protected:
    ClassifierFactory classifierFactory{};
};

TEST_F(ClassifierFactoryTest, build_svm_no_throw)
{
    EXPECT_NO_THROW(classifierFactory.buildSvm());
}

TEST_F(ClassifierFactoryTest, build_gaclassifier_with_svm_no_throw)
{
    EXPECT_NO_THROW(classifierFactory.buildGaClassifier(CLASSIFIER, TRAINING_SET_SPLIT_RATE, MUTATION_RATE, BIT_SWAP_RATE,
        PRESERVATION_RATE, GENERATIONS_NUMBER, POPULATION_SIZE, INITIAL_FILLUP));
}

}
