/*
 * ExtremaFinderTest.cpp
 * Tests whether peaks and valleys of signal are properly being found.
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
#include "Spectre.libPeakFinder\ExtremaFinder.h"

namespace
{
    using namespace spectre::algorithm::peakfinder;

    TEST(ExtremaFinderInitialization, initializes)
    {
        EXPECT_NO_THROW(ExtremaFinder());
    }

    class ExtremaFinderTest : public ::testing::Test
    {
    public:
        ExtremaFinderTest()
        {
            signal = {
                1.0, 2.0, 2.5, 2.8, 3.5, 2.8, 4.0, 3.0,
                3.0, 1.0, 1.5, 2.0, 2.5, 2.0, 1.0, 0.0
            };

            steepDownhillStartingSignal = {
                3.0, 1.0, 5.0, 1.0
            };
            steepUphillEndingSignal = {
                0.0, 5.0, 1.0, 10.0
            };
        }
    protected:
        ExtremaFinder finder;
        Signal signal;
        Signal steepDownhillStartingSignal;
        Signal steepUphillEndingSignal;
    };

    TEST_F(ExtremaFinderTest, finds_more_valleys_than_peaks)
    {
        Indices valleys = finder.FindValleys(signal);
        Indices peaks = finder.FindPeaks(signal);
        ASSERT_EQ(valleys.size() - 1, peaks.size());
    }

    TEST_F(ExtremaFinderTest, properly_finds_valleys)
    {
        Indices indices = finder.FindValleys(signal);
        Indices correct = { 0, 5, 9, 15 };
        ASSERT_EQ(correct, indices);
    }

    TEST_F(ExtremaFinderTest, doesnt_treat_first_peak_as_valley)
    {
        Indices indices = finder.FindValleys(steepDownhillStartingSignal);
        Indices correct = { 1, 3 };
        ASSERT_EQ(correct, indices);
    }

    TEST_F(ExtremaFinderTest, doesnt_treat_last_peak_as_valley)
    {
        Indices indices = finder.FindValleys(steepUphillEndingSignal);
        Indices correct = { 0, 2 };
        ASSERT_EQ(correct, indices);
    }

    TEST_F(ExtremaFinderTest, properly_finds_peaks)
    {
        Indices indices = finder.FindPeaks(signal);
        Indices correct = { 4, 6, 12 };
        ASSERT_EQ(correct, indices);
    }

    TEST_F(ExtremaFinderTest, ignores_peak_on_initial_downhill)
    {
        Indices indices = finder.FindPeaks(steepDownhillStartingSignal);
        Indices correct = { 2 };
        ASSERT_EQ(correct, indices);
    }

    TEST_F(ExtremaFinderTest, ignores_peak_on_final_uphill)
    {
        Indices indices = finder.FindPeaks(steepUphillEndingSignal);
        Indices correct = { 1 };
        ASSERT_EQ(correct, indices);
    }
}
