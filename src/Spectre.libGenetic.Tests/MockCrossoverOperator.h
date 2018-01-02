/*
* MockCrossoverOperator.h
* Mock for a crossover operator.
*
Copyright 2017 Grzegorz Mrukwa

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

#pragma once
#include <gmock/gmock.h>
#include "Spectre.libGenetic/CrossoverOperator.h"

namespace spectre::algorithm::genetic::Tests
{
class MockCrossoverOperator: public CrossoverOperator
{
public:
    MOCK_METHOD2(CallOperator, Individual(const Individual&, const Individual&));
    Individual operator()(const Individual &first, const Individual &second) override { return CallOperator(first, second); }
};
}
