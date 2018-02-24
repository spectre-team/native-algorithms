/*
* IndividualFeasibilityConditionsFactoryTest.cpp
* Tests IndividualFeasibilityConditionsFactory class.
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

#include "Spectre.libGaClassifier/IndividualFeasibilityConditionsFactory.h"
#include <gtest/gtest.h>
#include "Spectre.libGenetic/InconsistentChromosomeLengthException.h"
#include "Spectre.libGaClassifier/InconsistentMinimalAndMaximalFillupException.h"

namespace
{
using namespace spectre::supervised;
using namespace spectre::algorithm::genetic;

TEST(IndividualFeasibilityConditionsFactoryInitialization, initializes)
{
    EXPECT_NO_THROW(IndividualFeasibilityConditionsFactory());
}

TEST(IndividualFeasibilityConditionsFactoryInitialization, throws_on_wrong_fillups)
{
    EXPECT_THROW(IndividualFeasibilityConditionsFactory(0, 3, 2), InconsistentMinimalAndMaximalFillupException);
}

TEST(IndividualFeasibilityConditionsFactoryInitialization, throws_on_wrong_percentage_fillups)
{
    EXPECT_THROW(IndividualFeasibilityConditionsFactory(0, 1, 4, 0.6, 0.4), InconsistentMinimalAndMaximalFillupException);
}

class IndividualFeasibilityConditionsFactoryTest : public ::testing::Test
{
public:
    IndividualFeasibilityConditionsFactoryTest():
        individual(std::vector<bool>(data)),
        tooManyTrueIndividual(std::vector<bool>(tooManyTrueData)),
        toofewTrueIndividual(std::vector<bool>(toofewTrueData)),
        oneLabelIndividual(std::vector<bool>(oneLabelData)),
        factory(length, minimalFillup, maximalFillup, minimalPercentageFillup, maximalPercentageFillup, allLabel, labels) {}

protected:
    const std::vector<bool> data{ true, false, true, true, false };
    const Individual individual;
    const std::vector<bool> tooManyTrueData{ true, true, true, true, true };
    const Individual tooManyTrueIndividual;
    const std::vector<bool> toofewTrueData{ true, false, false, false, true };
    const Individual toofewTrueIndividual;
    const std::vector<bool> oneLabelData{ false, true, true, false, true };
    const Individual oneLabelIndividual;
    const std::vector<spectre::supervised::Label> labels{ true, false, false, true, false };
    size_t length = 5;
    size_t minimalFillup = 1;
    size_t maximalFillup = 4;
    double minimalPercentageFillup = 0.6;
    double maximalPercentageFillup = 1;
    bool allLabel = true;
    IndividualFeasibilityConditionsFactory factory;
};

TEST_F(IndividualFeasibilityConditionsFactoryTest, correct_individual_feasibility_condition_build)
{
    EXPECT_NO_THROW(factory.build());
}

TEST_F(IndividualFeasibilityConditionsFactoryTest, condition_created_passes)
{
    std::unique_ptr<BaseIndividualFeasibilityCondition> condition = factory.build();
    EXPECT_TRUE(condition->check(individual));
}

TEST_F(IndividualFeasibilityConditionsFactoryTest, condition_created_fails_on_too_many_true_data)
{
    std::unique_ptr<BaseIndividualFeasibilityCondition> condition = factory.build();
    EXPECT_FALSE(condition->check(tooManyTrueIndividual));
}

TEST_F(IndividualFeasibilityConditionsFactoryTest, condition_created_fails_on_too_many_few_data)
{
    std::unique_ptr<BaseIndividualFeasibilityCondition> condition = factory.build();
    EXPECT_FALSE(condition->check(toofewTrueIndividual));
}

TEST_F(IndividualFeasibilityConditionsFactoryTest, condition_created_fails_on_only_one_label)
{
    std::unique_ptr<BaseIndividualFeasibilityCondition> condition = factory.build();
    EXPECT_FALSE(condition->check(oneLabelIndividual));
}

}
