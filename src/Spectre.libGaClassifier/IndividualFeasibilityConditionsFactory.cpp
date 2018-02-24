/*
* IndividualFeasibilityConditionsFactory.cpp
* Creates individual feasibility condition chain.
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

#include "IndividualFeasibilityConditionsFactory.h"
#include "InconsistentMinimalAndMaximalFillupException.h"
#include "Spectre.libGenetic/LengthCondition.h"
#include "Spectre.libGenetic/MinimalFillupCondition.h"
#include "Spectre.libGenetic/MaximalFillupCondition.h"
#include "Spectre.libGenetic/MinimalPercentageFillupCondition.h"
#include "Spectre.libGenetic/MaximalPercentageFillupCondition.h"
#include "AllLabelTypesIncludedCondition.h"

namespace spectre::supervised
{
IndividualFeasibilityConditionsFactory::IndividualFeasibilityConditionsFactory(size_t length, size_t minimalFillup, size_t maximalFillup, double minimalPercentageFillup,
                                                                               double maximalPercentageFillup, bool allLabelTypes, gsl::span<const supervised::Label> labels):
    m_Length(length),
    m_MinimalFillup(minimalFillup),
    m_MaximalFillup(maximalFillup),
    m_MinimalPercentageFillup(minimalPercentageFillup),
    m_MaximalPercentageFillup(maximalPercentageFillup),
    m_AllLabelTypes(allLabelTypes),
    m_Labels(labels)
{
    if (m_MinimalFillup > m_MaximalFillup)
    {
        throw InconsistentMinimalAndMaximalFillupException(m_MinimalFillup, m_MaximalFillup);
    }
    if (m_MinimalPercentageFillup > m_MaximalPercentageFillup)
    {
        throw InconsistentMinimalAndMaximalFillupException(m_MinimalPercentageFillup, m_MaximalPercentageFillup);
    }
}

std::unique_ptr<algorithm::genetic::BaseIndividualFeasibilityCondition> IndividualFeasibilityConditionsFactory::build()
{
    std::unique_ptr<algorithm::genetic::BaseIndividualFeasibilityCondition> condition = nullptr;
    if (m_Length != 0)
    {
        condition = std::make_unique<algorithm::genetic::LengthCondition>(m_Length, std::move(condition));
    }
    if (m_MinimalFillup != 0)
    {
        condition = std::make_unique<algorithm::genetic::MinimalFillupCondition>(m_MinimalFillup, std::move(condition));
    }
    if (m_MaximalFillup != UINT_MAX)
    {
        condition = std::make_unique<algorithm::genetic::MaximalFillupCondition>(m_MaximalFillup, std::move(condition));
    }
    if (m_MinimalPercentageFillup != 0)
    {
        condition = std::make_unique<algorithm::genetic::MinimalPercentageFillupCondition>(m_MinimalPercentageFillup, std::move(condition));
    }
    if (m_MaximalPercentageFillup != 1)
    {
        condition = std::make_unique<algorithm::genetic::MaximalPercentageFillupCondition>(m_MaximalPercentageFillup, std::move(condition));
    }
    if (m_AllLabelTypes == true)
    {
        condition = std::make_unique<AllLabelTypesIncludedCondition>(m_Labels, std::move(condition));
    }
    return condition;
}

}
