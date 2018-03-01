/*
* MinimalPercentageFillupConditionTest.cpp
* Tests MinimalPercentageFillupCondition class.
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
#include "Spectre.libGenetic/MinimalPercentageFillupCondition.h"

namespace
{
using namespace spectre::algorithm::genetic;

TEST(MinimalPercentageFillupConditionInitialization, initializes_with_one_parameter)
{
    double fillup = 1;
    EXPECT_NO_THROW(MinimalPercentageFillupCondition condition(fillup));
}

TEST(MinimalPercentageFillupConditionInitialization, initializes_with_two_parameters)
{
    double fillup = 1;
    std::unique_ptr<MinimalPercentageFillupCondition> firstCondition = std::make_unique<MinimalPercentageFillupCondition>(fillup);
    EXPECT_NO_THROW(MinimalPercentageFillupCondition condition(fillup, std::move(firstCondition)));
}

class MinimalPercentageFillupConditionTest : public ::testing::Test
{
public:
    MinimalPercentageFillupConditionTest() {}
protected:
    const double fillup = 0.5;
    const Individual mostlyTrueIndividual = Individual({ false, false, true, true, false, true, true });
    const Individual mostlyFalseIndividual = Individual({ true, false, false, true, false, true, false });
};

TEST_F(MinimalPercentageFillupConditionTest, return_true_for_correct_single_condition)
{
    MinimalPercentageFillupCondition condition(fillup);
    auto result = condition.checkCurrentCondition(mostlyTrueIndividual);
    EXPECT_TRUE(result);
}

TEST_F(MinimalPercentageFillupConditionTest, return_false_for_too_low_fillup_in_individual)
{
    MinimalPercentageFillupCondition condition(fillup);
    auto result = condition.checkCurrentCondition(mostlyFalseIndividual);
    EXPECT_FALSE(result);
}
}
