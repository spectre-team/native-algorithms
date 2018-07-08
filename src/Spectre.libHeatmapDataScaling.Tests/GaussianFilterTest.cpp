/*
* GaussianFilterTest.cpp
* Tests GaussianFilter
*
Copyright 2018 Daniel Babiak

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
#include <gmock/gmock-matchers.h>
#include "Spectre.libHeatmapDataScaling/GaussianFilter.h"

using namespace spectre::visualization;

namespace
{
    class GaussianFilterTest : public ::testing::Test
    {
    protected:
        GaussianFilter gaussianFilter;
        std::vector<double> testIntensities;
        void SetUp() override
        {
            
        }
    };
}