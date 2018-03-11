/*
* FWWHHCalculatorTest.cpp
* Test for calculations of full width at half height markers.
*
Copyright 2018 Dariusz Kuchta

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
#include "Spectre.libPeakFinder/FWHHCalculator.h"
#include <gmock/gmock-matchers.h>

namespace
{
    using namespace spectre::algorithm::peakfinder;
    using namespace testing;
    using namespace gsl;

    class FWHHCalculatorTest : public Test
    {
    public:
        FWHHCalculatorTest()
        {
            x = { 1., 2., 3., 4., 5., 6., 7., 8., 9. };

            sawtoothY = { 0., 1., 2., 1., 0., 1., 2., 1., 0. };
            sawtoothValleyIdx = { 0, 4, 8 };
            sawtoothPeakIdx = { 2, 6 };
        }

    protected:
        FWHHCalculator fwhh;
        Signal x;

        Signal sawtoothY;
        Indices sawtoothValleyIdx;
        Indices sawtoothPeakIdx;
    };

    TEST_F(FWHHCalculatorTest, sawtooth_correct_left_fwhh)
    {
        Signal result = fwhh.GetLeftFWHH(as_span(x), as_span(sawtoothY),
            as_span(sawtoothValleyIdx), as_span(sawtoothPeakIdx));
        EXPECT_THAT(result, ContainerEq(Signal{ 2., 6. }));
    }

    TEST_F(FWHHCalculatorTest, sawtooth_correct_right_fwhh)
    {
        Signal result = fwhh.GetRightFWHH(as_span(x), as_span(sawtoothY),
            as_span(sawtoothValleyIdx), as_span(sawtoothPeakIdx));
        EXPECT_THAT(result, ContainerEq(Signal{ 4., 8. }));
    }
}