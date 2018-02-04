/*
* MinimalTrueValueNumberConditionTest.cpp
* Tests MinimalTrueValueNumberCondition class.
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
#include "Spectre.libGenetic/MinimalTrueValueNumberCondition.h"

namespace
{
    using namespace spectre::algorithm::genetic;

    TEST(MinimalTrueValueNumberConditionInitialization, initializes_with_one_parameter)
    {
        size_t len = 1;
        EXPECT_NO_THROW(MinimalTrueValueNumberCondition condition(len));
    }

    TEST(MinimalTrueValueNumberConditionInitialization, initializes_with_two_parameters)
    {
        size_t len = 1;
        std::unique_ptr<MinimalTrueValueNumberCondition> firstCondition = std::make_unique<MinimalTrueValueNumberCondition>(len);
        EXPECT_NO_THROW(MinimalTrueValueNumberCondition condition(len, std::move(firstCondition)));
    }

    class MinimalTrueValueNumberConditionTest : public ::testing::Test
    {
    public:
        MinimalTrueValueNumberConditionTest() {}
    protected:
        const size_t lowLen = 2;
        const size_t highLen = 5;
        const Individual longIndividual = Individual({ true, true, true, false, true, false, true, false });
        const Individual mediumIndividual = Individual({ false, false, true, true, false, true, true });
        const Individual shortIndividual = Individual({ true, false, false });

        void SetUp() override
        {
        }
    };

    TEST_F(MinimalTrueValueNumberConditionTest, return_true_for_correct_single_condition)
    {
        MinimalTrueValueNumberCondition condition(lowLen);
        auto result = condition.privateConditionCheck(mediumIndividual);
        EXPECT_TRUE(result);
    }

    TEST_F(MinimalTrueValueNumberConditionTest, return_short_for_too_short_individual_in_single_condition)
    {
        MinimalTrueValueNumberCondition condition(lowLen);
        auto result = condition.privateConditionCheck(shortIndividual);
        EXPECT_FALSE(result);
    }

    TEST_F(MinimalTrueValueNumberConditionTest, return_true_for_correct_double_condition)
    {
        std::unique_ptr<MinimalTrueValueNumberCondition> firstCondition = std::make_unique<MinimalTrueValueNumberCondition>(highLen);
        MinimalTrueValueNumberCondition secondCondition(lowLen, std::move(firstCondition));
        auto result = secondCondition.checkCondition(longIndividual);
        EXPECT_TRUE(result);
    }

    TEST_F(MinimalTrueValueNumberConditionTest, return_false_for_too_short_individual_for_first_condition)
    {
        std::unique_ptr<MinimalTrueValueNumberCondition> firstCondition = std::make_unique<MinimalTrueValueNumberCondition>(highLen);
        MinimalTrueValueNumberCondition secondCondition(lowLen, std::move(firstCondition));
        auto result = secondCondition.checkCondition(shortIndividual);
        EXPECT_FALSE(result);
    }

    TEST_F(MinimalTrueValueNumberConditionTest, return_false_for_too_short_individual_for_second_condition)
    {
        std::unique_ptr<MinimalTrueValueNumberCondition> firstCondition = std::make_unique<MinimalTrueValueNumberCondition>(highLen);
        MinimalTrueValueNumberCondition secondCondition(lowLen, std::move(firstCondition));
        auto result = secondCondition.checkCondition(mediumIndividual);
        EXPECT_FALSE(result);
    }

}
