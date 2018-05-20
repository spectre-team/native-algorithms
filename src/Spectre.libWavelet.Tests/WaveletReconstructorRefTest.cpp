/*
* WaveletReconstructorRefTest.cpp
* Tests whether the signal is properly reconstructed by the reference
* implementation.
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
#include "Spectre.libWavelet/WaveletReconstructorRef.h"
#include "Spectre.libWavelet/WaveletDecomposerRef.h"
#include "Spectre.libFunctional/Range.h"
#include "FloatingPointVectorMatcher.h"

namespace
{
using namespace spectre::algorithm::wavelet;
using namespace spectre;

TEST(WaveletReconstructorInitialization, initializes)
{
    EXPECT_NO_THROW(WaveletReconstructorRef());
}

class WaveletReconstructorRefTest : public ::testing::Test
{
public:
    WaveletReconstructorRefTest()
    {
    }

protected:
    WaveletReconstructorRef reconstructor;
    WaveletDecomposerRef decomposer;
};

TEST_F(WaveletReconstructorRefTest, reconstructs_the_signal)
{
    // Note: Normally tests shouldn't rely on functionality of other classes.
    // However, function Reconstruct accepts a set of 11 x 512 x 15 coefficients
    // which would be hard to reproduce both by hand and algoritmically without
    // reimplementing the wavelet decomposer.

    Data signal = spectre::core::functional::range<DataType>(10);
    Data correct = signal; // Copy before the signal get's moved
    WaveletCoefficients coefficients = decomposer.Decompose(std::move(signal));
    Data reconstructed = reconstructor.Reconstruct(std::move(coefficients), correct.size());

    auto doubleNear = double_near(0.001);
    EXPECT_THAT(correct, testing::Pointwise(doubleNear, reconstructed));
}
}
