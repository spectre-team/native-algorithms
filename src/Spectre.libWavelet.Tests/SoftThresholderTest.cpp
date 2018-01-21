/*
* SoftThresholderTest.cpp
* Testing of the soft tresholding.
*
Copyright 2018 Michal Gallus

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
#include "Spectre.libWavelet\SoftThresholder.h"

namespace
{
    using namespace spectre::algorithm::wavelet;

    TEST(SoftThresholderInitialization, initializes)
    {
        EXPECT_NO_THROW(SoftThresholder(0.0));
        EXPECT_NO_THROW(SoftThresholder(9.0));
        EXPECT_NO_THROW(SoftThresholder(-11.0));
    }

    class SoftThresholderTest : public ::testing::Test
    {
    public:
        SoftThresholderTest() : tresholder(1.0) {}
    protected:
        SoftThresholder tresholder;
    };

    TEST_F(SoftThresholderTest, properly_tresholds_signal)
    {
        Signal input = { 1.0f, 2.0f, 3.0f, 4.0f };
        Signal output = tresholder(input);
        Signal correctOutput = { 0.0f, 1.0f, 2.0f, 3.0f };
        ASSERT_EQ(correctOutput, output);
    }
}
