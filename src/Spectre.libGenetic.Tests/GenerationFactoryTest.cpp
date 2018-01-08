/*
* GenerationFactoryTest.cpp
* Tests GenerationFactory.
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
#include "Spectre.libGenetic/GenerationFactory.h"
#include "Spectre.libException/ArgumentOutOfRangeException.h"

namespace
{
using namespace spectre::algorithm::genetic;
using namespace spectre::core::exception;

TEST(GenerationFactoryInitialization, initializes)
{
    const auto generationSize = 5u;
    const auto individualSize = 6u;
    const auto initialFillup = 4u;
    EXPECT_NO_THROW(GenerationFactory(generationSize, individualSize, initialFillup));
}

TEST(GenerationInitialization, throws_when_required_number_of_active_is_bigger_than_length)
{
    const auto generationSize = 5u;
    const auto individualSize = 4u;
    const auto excessiveInitialFillup = 6u;
    EXPECT_THROW(GenerationFactory(generationSize, individualSize, excessiveInitialFillup), ArgumentOutOfRangeException<size_t>);
}

class GenerationFactoryInitializationTest : public ::testing::Test
{
public:
    GenerationFactoryInitializationTest():
        generationFatory(generationSize, individualSize, initialFillup),
        generationFatoryWithBigIndividuals(bigGenerationSize, bigIndividualSize, bigInitialFillup) {}

protected:
    const size_t generationSize = 1u;
    const size_t individualSize = 6u;
    const size_t initialFillup = 4u;
    const size_t bigGenerationSize = 2u;
    const size_t bigIndividualSize = 100u;
    const size_t bigInitialFillup = 60u;
    const Seed seed = 0ul;
    const Seed seed1 = 5u;
    const Seed seed2 = 6u;
    GenerationFactory generationFatory;
    GenerationFactory generationFatoryWithBigIndividuals;

    void SetUp() override
    {
        generationFatory = GenerationFactory(generationSize, individualSize, initialFillup);
        generationFatoryWithBigIndividuals = GenerationFactory(bigGenerationSize, bigIndividualSize, bigInitialFillup);
    }
};

TEST_F(GenerationFactoryInitializationTest, true_amount_equal_to_parameter)
{
    Generation testGeneration = generationFatory(seed);
    for (auto i = 0u; i < testGeneration.size(); i++)
    {
        auto trueAmount = 0u;
        for (auto j = 0u; j < testGeneration[i].size(); j++)
        {
            if (testGeneration[i][j] == true)
            {
                trueAmount++;
            }
        }
        EXPECT_EQ(trueAmount, initialFillup);
    }
}

TEST_F(GenerationFactoryInitializationTest, create_different_individuals_for_different_seeds)
{
    Generation testGeneration = generationFatoryWithBigIndividuals(seed);
    bool different = false;

    EXPECT_EQ(testGeneration[0].size(), testGeneration[1].size());
    for (auto i = 0u; i < testGeneration[0].size(); i++)
    {
        if (testGeneration[0][i] != testGeneration[1][i])
        {
            different = true;
            break;
        }
    }
    EXPECT_TRUE(different);
}

}
