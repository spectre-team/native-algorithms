/*
* AllLabelTypesIncludedConditionTest.cpp
* Tests AllLabelTypesIncludedCondition class.
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
#include "Spectre.libGaClassifier/AllLabelTypesIncludedCondition.h"

namespace
{
using namespace spectre::supervised;

TEST(AllLabelTypesIncludedConditionInitialization, initializes_with_one_parameter)
{
    const std::vector<spectre::supervised::Label> labels{ true, true, true, true, true };
    EXPECT_NO_THROW(AllLabelTypesIncludedCondition condition(labels));
}

class AllLabelTypesIncludedConditionTest : public ::testing::Test
{
public:
    AllLabelTypesIncludedConditionTest() {}
protected:
    const spectre::algorithm::genetic::Individual individual = spectre::algorithm::genetic::Individual({ true, true, true, true, true });
    const std::vector<spectre::supervised::Label> allTrueLabels{ true, true, true, true, true };
    const std::vector<spectre::supervised::Label> allFalseLabels{ false, false, false, false, false };
    const std::vector<spectre::supervised::Label> trueFalseLabels{ true, false, true, true, false };
};

TEST_F(AllLabelTypesIncludedConditionTest, return_true_for_correct_label)
{
    AllLabelTypesIncludedCondition condition(trueFalseLabels);
    auto result = condition.checkCurrentCondition(individual);
    EXPECT_TRUE(result);
}

TEST_F(AllLabelTypesIncludedConditionTest, return_false_for_all_true_label)
{
    AllLabelTypesIncludedCondition condition(allTrueLabels);
    auto result = condition.checkCurrentCondition(individual);
    EXPECT_FALSE(result);
}

TEST_F(AllLabelTypesIncludedConditionTest, return_false_for_all_false_label)
{
    AllLabelTypesIncludedCondition condition(allFalseLabels);
    auto result = condition.checkCurrentCondition(individual);
    EXPECT_FALSE(result);
}
}
