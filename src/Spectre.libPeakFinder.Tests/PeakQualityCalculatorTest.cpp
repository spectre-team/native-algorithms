/*
* PeakQualityCalculatorTest.cpp
* Tests for PeakQualityCalculator class.
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
#include "Spectre.libPeakFinder/PeakQualityCalculator.h"
#include <gmock/gmock-matchers.h>


namespace
{
    using namespace spectre::algorithm::peakfinder;
    using namespace spectre;
    using namespace testing;

    class PeakQualityCalculatorTest : public Test
    {
    public:
        PeakQualityCalculatorTest()
        {
            peakIntensities = { 5., 8., 6., 3. };
            valleyIntensities = { 2., 4., 1., 1., 2. };
        }
    protected:
        PeakQualityCalculator pqc;

        Data peakIntensities;
        Data valleyIntensities;
    };

    TEST_F(PeakQualityCalculatorTest, finds_correct_qualities)
    {
        Data result = pqc.CalculatePeakQualities(peakIntensities, valleyIntensities);
        EXPECT_THAT(result, ContainerEq(Data{1., 1.6, 3., 1.}));
    }
}

