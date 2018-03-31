/*
* ClearPeakFinderTest.cpp
* Tests finding of splitting peaks among the list of peaks.
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
#define GTEST_LANG_CXX11 1

#include <gtest/gtest.h>
#include "ClearPeakFinder.h"
#include "Spectre.libException\EmptyArgumentException.h"
#include "Spectre.libException\InconsistentArgumentSizesException.h"

namespace spectre::unsupervised::gmm
{
using namespace core::exception;
class ClearPeakFinderTest : public ::testing::Test
{
public:
    ClearPeakFinderTest()
    {
        peaks.mzs = {
            2.0184, 2.0448, 2.0712, 2.0848, 2.0920,
            2.1096, 2.1344, 2.1728, 2.1872, 2.1952,
            2.2200, 2.2416, 2.2576, 2.3000, 2.3368,
            2.3712, 2.4160, 2.4400, 2.5009, 2.5257
        };
        peaks.intensities = {
            0.0006, 0.0003, 0.0122, 0.0003, 0.0004,
            0.0006, 0.0007, 0.0092, 0.0005, 0.0004,
            0.0223, 0.0005, 0.0005, 0.1317, 0.0003,
            0.0765, 0.0258, 0.1851, 0.0322, 0.0439
        };
        peakQualities = {
            00.4503, 0.2564, 10.7977, 000.2979, 00.3453,
            00.5217, 0.6438, 08.5515, 000.4669, 00.3708,
            21.8366, 0.4437, 00.5053, 127.7454, 00.3045,
            72.3374, 5.2706, 37.8574, 010.9249, 14.8931
        };
    }
protected:
    ClearPeakFinder finder;
    Data peakQualities;
    Peaks peaks;
};

TEST_F(ClearPeakFinderTest, correctly_finds_clear_peaks)
{
    GmmOptions options;
    DataType resolutionCoeff = 0.0027;
    Indices indices = finder.FindClearPeaks(peaks, peakQualities, options, resolutionCoeff);
    Indices correct = { 7, 13 };
    ASSERT_EQ(correct, indices);
}

TEST_F(ClearPeakFinderTest, throws_on_empty_peak_list)
{
    GmmOptions options;
    DataType resolutionCoeff = 0.0027;
    EXPECT_THROW(finder.FindClearPeaks({}, peakQualities, options, resolutionCoeff), EmptyArgumentException);
}

TEST_F(ClearPeakFinderTest, throws_on_empty_quality_peak_list)
{
    GmmOptions options;
    DataType resolutionCoeff = 0.0027;
    EXPECT_THROW(finder.FindClearPeaks(peaks, {}, options, resolutionCoeff), EmptyArgumentException);
}

TEST_F(ClearPeakFinderTest, throws_on_different_peak_and_quality_list_sizes)
{
    GmmOptions options;
    DataType resolutionCoeff = 0.0027;
    Data qualities = { 0.0 };
    EXPECT_THROW(finder.FindClearPeaks(peaks, qualities, options, resolutionCoeff), InconsistentArgumentSizesException);
}

TEST_F(ClearPeakFinderTest, returns_empty_list_when_there_are_no_peaks)
{
    GmmOptions options;
    DataType resolutionCoeff = 0.0027;
    Peaks peaks2;
    peaks2.mzs = Data(50);
    peaks2.intensities = Data(50); // All filled with zeros = no peaks.
    Data qualities = Data(50);
    auto test = finder.FindClearPeaks(peaks2, qualities, options, resolutionCoeff);
}
}
