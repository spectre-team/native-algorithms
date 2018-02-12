/*
* MinimalFillupConditionTest.cpp
* Tests MinimalFillupCondition class.
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
#include "Spectre.libGenetic/MinimalFillupCondition.h"

namespace
{
using namespace spectre::algorithm::genetic;

TEST(MinimalFillupConditionInitialization, initializes_with_one_parameter)
{
    size_t len = 1;
    EXPECT_NO_THROW(MinimalFillupCondition condition(len));
}

TEST(MinimalFillupConditionInitialization, initializes_with_two_parameters)
{
    size_t len = 1;
    std::unique_ptr<MinimalFillupCondition> firstCondition = std::make_unique<MinimalFillupCondition>(len);
    EXPECT_NO_THROW(MinimalFillupCondition condition(len, std::move(firstCondition)));
}

class MinimalFillupConditionTest : public ::testing::Test
{
public:
    MinimalFillupConditionTest() {}
protected:
    const size_t lowLen = 2;
    const Individual mediumIndividual = Individual({ false, false, true, true, false, true, true });
    const Individual shortIndividual = Individual({ true, false, false });

    void SetUp() override
    {
    }
};

TEST_F(MinimalFillupConditionTest, return_true_for_correct_single_condition)
{
    MinimalFillupCondition condition(lowLen);
    auto result = condition.currentConditionCheck(mediumIndividual);
    EXPECT_TRUE(result);
}

TEST_F(MinimalFillupConditionTest, return_false_for_too_low_fillup_in_individual)
{
    MinimalFillupCondition condition(lowLen);
    auto result = condition.currentConditionCheck(shortIndividual);
    EXPECT_FALSE(result);
}
}
