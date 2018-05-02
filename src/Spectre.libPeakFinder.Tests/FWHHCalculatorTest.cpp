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
    using namespace spectre;
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

            skewedY = { 2., 0., 1., 2., 3., 4., 2., 0., 1. };
            skewedValleyIdx = { 1, 7 };
            skewedPeakIdx = { 5 };
        }

    protected:
        FWHHCalculator fwhh;
        Data x;

        Data sawtoothY;
        Indices sawtoothValleyIdx;
        Indices sawtoothPeakIdx;

        Data skewedY;
        Indices skewedValleyIdx;
        Indices skewedPeakIdx;
    };

    TEST_F(FWHHCalculatorTest, sawtooth_correct_left_fwhh)
    {
        Data result = fwhh.GetLeftFWHH(make_span(x), make_span(sawtoothY),
            make_span(sawtoothValleyIdx), make_span(sawtoothPeakIdx));
        EXPECT_THAT(result, ContainerEq(Data{ 2., 6. }));
    }

    TEST_F(FWHHCalculatorTest, sawtooth_correct_right_fwhh)
    {
        Data result = fwhh.GetRightFWHH(make_span(x), make_span(sawtoothY),
            make_span(sawtoothValleyIdx), make_span(sawtoothPeakIdx));
        EXPECT_THAT(result, ContainerEq(Data{ 4., 8. }));
    }

    TEST_F(FWHHCalculatorTest, skewed_correct_left_fwhh)
    {
        Data result = fwhh.GetLeftFWHH(make_span(x), make_span(skewedY),
            make_span(skewedValleyIdx), make_span(skewedPeakIdx));
        EXPECT_THAT(result, ContainerEq(Data{ 4. }));
    }

    TEST_F(FWHHCalculatorTest, skewed_correct_right_fwhh)
    {
        Data result = fwhh.GetRightFWHH(make_span(x), make_span(skewedY),
            make_span(skewedValleyIdx), make_span(skewedPeakIdx));
        EXPECT_THAT(result, ContainerEq(Data{ 7. }));
    }
}
