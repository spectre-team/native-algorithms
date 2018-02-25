/*
* MaximalFillupConditionTest.cpp
* Tests MaximalFillupCondition class.
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
#include "Spectre.libGenetic/MaximalFillupCondition.h"

namespace
{
using namespace spectre::algorithm::genetic;

TEST(MaximalFillupConditionInitialization, initializes_with_one_parameter)
{
    size_t len = 1;
    EXPECT_NO_THROW(MaximalFillupCondition condition(len));
}

TEST(MaximalFillupConditionInitialization, initializes_with_two_parameters)
{
    size_t len = 1;
    std::unique_ptr<MaximalFillupCondition> firstCondition = std::make_unique<MaximalFillupCondition>(len);
    EXPECT_NO_THROW(MaximalFillupCondition condition(len, std::move(firstCondition)));
}

class MaximalFillupConditionTest : public ::testing::Test
{
public:
    MaximalFillupConditionTest() {}
protected:
    const size_t lowLen = 2;
    const Individual mediumIndividual = Individual({ false, false, true, true, false, true, true });
    const Individual shortIndividual = Individual({ true, false, false });
};

TEST_F(MaximalFillupConditionTest, return_true_for_correct_single_condition)
{
    MaximalFillupCondition condition(lowLen);
    auto result = condition.checkCurrentCondition(shortIndividual);
    EXPECT_TRUE(result);
}

TEST_F(MaximalFillupConditionTest, return_false_for_too_high_fillup_in_individual)
{
    MaximalFillupCondition condition(lowLen);
    auto result = condition.checkCurrentCondition(mediumIndividual);
    EXPECT_FALSE(result);
}
}
