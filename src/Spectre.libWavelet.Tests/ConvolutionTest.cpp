/*
* ConvolutionTest.cpp
* Tests convolution of the signal with given filter.
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
#include "Spectre.libWavelet\Convolution.h"
#include "Spectre.libFunctional\Range.h"

namespace
{
    using namespace spectre::algorithm::wavelet;
    using namespace spectre;

    TEST(ConvolutionInitialization, initializes)
    {
        EXPECT_NO_THROW(Convolution());
    }

    class ConvolutionTest : public ::testing::Test
    {
    protected:
        Convolution convolution;
    };

    TEST_F(ConvolutionTest, properly_convolves_the_kernel_over_a_signal)
    {
        Data signal = spectre::core::functional::range<DataType>(10);
        Kernel kernel = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
        Data result = convolution.Convolve(kernel, signal);
        Data correctResult = { 0.0, 0.0, 1.0, 4.0, 10.0, 20.0, 35.0, 56.0, 84.0, 112.0 };
        ASSERT_EQ(correctResult, result);
    }
}
