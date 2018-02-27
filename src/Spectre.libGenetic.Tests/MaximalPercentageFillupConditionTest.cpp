/*
* MaximalPercentageFillupConditionTest.cpp
* Tests MaximalPercentageFillupCondition class.
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
#include "Spectre.libGenetic/MaximalPercentageFillupCondition.h"

namespace
{
using namespace spectre::algorithm::genetic;

TEST(MaximalPercentageFillupConditionInitialization, initializes_with_one_parameter)
{
    double fillup = 1;
    EXPECT_NO_THROW(MaximalPercentageFillupCondition condition(fillup));
}

TEST(MaximalPercentageFillupConditionInitialization, initializes_with_two_parameters)
{
    double fillup = 1;
    std::unique_ptr<MaximalPercentageFillupCondition> firstCondition = std::make_unique<MaximalPercentageFillupCondition>(fillup);
    EXPECT_NO_THROW(MaximalPercentageFillupCondition condition(fillup, std::move(firstCondition)));
}

class MaximalPercentageFillupConditionTest : public ::testing::Test
{
public:
    MaximalPercentageFillupConditionTest() {}
protected:
    const double fillup = 0.5;
    const Individual mostlyTrueIndividual = Individual({ false, false, true, true, false, true, true });
    const Individual mostlyFalseIndividual = Individual({ true, false, false, true, false, true, false });
};

TEST_F(MaximalPercentageFillupConditionTest, return_true_for_correct_single_condition)
{
    MaximalPercentageFillupCondition condition(fillup);
    auto result = condition.checkCurrentCondition(mostlyFalseIndividual);
    EXPECT_TRUE(result);
}

TEST_F(MaximalPercentageFillupConditionTest, return_false_for_too_high_fillup_in_individual)
{
    MaximalPercentageFillupCondition condition(fillup);
    auto result = condition.checkCurrentCondition(mostlyTrueIndividual);
    EXPECT_FALSE(result);
}
}
