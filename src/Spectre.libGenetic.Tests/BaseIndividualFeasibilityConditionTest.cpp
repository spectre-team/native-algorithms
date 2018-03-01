/*
* BaseIndividualFeasibilityConditionTest.cpp
* Tests BaseIndividualFeasibilityCondition class.
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
#include "Spectre.libGenetic/BaseIndividualFeasibilityCondition.h"
#include "MockBaseIndividualFeasibilityCondition.h"

namespace
{
using namespace spectre::algorithm::genetic;

TEST(BaseIndividualFeasibilityConditionInitialization, initializes_with_one_parameter)
{
    EXPECT_NO_THROW(Tests::MockBaseIndividualFeasibilityCondition condition(nullptr));
}

TEST(BaseIndividualFeasibilityConditionInitialization, initializes_with_two_parameters)
{
    auto firstCondition = std::make_unique<Tests::MockBaseIndividualFeasibilityCondition>(nullptr);
    std::unique_ptr<Tests::MockBaseIndividualFeasibilityCondition> secondCondition(std::move(firstCondition));
    EXPECT_NO_THROW(std::unique_ptr<Tests::MockBaseIndividualFeasibilityCondition> thirdCondition(std::move(secondCondition)));
}

TEST(BaseIndividualFeasibilityConditionInitialization, return_true_for_chained_condition_check)
{
    Individual test({ true });
    auto firstCondition = std::make_unique<Tests::MockBaseIndividualFeasibilityCondition>(nullptr);
    EXPECT_CALL(*firstCondition, checkCurrentCondition(test)).WillOnce(testing::Return(true));

    Tests::MockBaseIndividualFeasibilityCondition secondCondition(std::move(firstCondition));
    EXPECT_CALL(secondCondition, checkCurrentCondition(test)).WillOnce(testing::Return(true));

    EXPECT_TRUE(secondCondition.check(test));
}

TEST(BaseIndividualFeasibilityConditionInitialization, return_false_for_first_condition_not_fulfilled)
{
    Individual test({ true });
    auto firstCondition = std::make_unique<Tests::MockBaseIndividualFeasibilityCondition>(nullptr);
    EXPECT_CALL(*firstCondition, checkCurrentCondition(test)).WillOnce(testing::Return(false));

    Tests::MockBaseIndividualFeasibilityCondition secondCondition(std::move(firstCondition));
    EXPECT_CALL(secondCondition, checkCurrentCondition(test)).WillOnce(testing::Return(true));

    EXPECT_FALSE(secondCondition.check(test));
}

TEST(BaseIndividualFeasibilityConditionInitialization, return_false_for_second_condition_not_fulfilled)
{
    Individual test({ true });
    auto firstCondition = std::make_unique<Tests::MockBaseIndividualFeasibilityCondition>(nullptr);

    Tests::MockBaseIndividualFeasibilityCondition secondCondition(std::move(firstCondition));
    EXPECT_CALL(secondCondition, checkCurrentCondition(test)).WillOnce(testing::Return(false));

    EXPECT_FALSE(secondCondition.check(test));
}
}
