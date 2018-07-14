/*
* SignalDecomposerTest.cpp
* Tests the functionality of dynamic programming intialization facilities
* for expectation-maximization algorithm.
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
#include "Spectre.libGaussianMixtureModelling/SplitterSegmentDecomposer.h"

namespace spectre::unsupervised::gmm
{
class SignalDecomposerTest : public ::testing::Test
{
public:
    SignalDecomposerTest()
    {
        mzs = {
            2056.43908092161, 2057.23916092961, 2058.03924093762, 2058.83932094562,
            2059.63940095362, 2060.43948096162, 2061.23956096962, 2062.03964097762,
            2062.83972098562, 2063.63980099362, 2064.43988100162, 2065.23996100962,
            2066.04004101762, 2066.84012102562, 2067.64020103363, 2068.44028104163,
            2069.24036104963, 2070.04044105763, 2070.84052106563, 2071.64060107363,
            2072.44068108163, 2073.24076108963, 2074.04084109763, 2074.84092110563,
            2075.64100111363, 2076.44108112163, 2077.24116112963, 2078.04124113764,
            2078.84132114564, 2079.64140115364, 2080.44148116164, 2081.24156116964,
            2082.04164117764, 2083.20832083208, 2084.00840084008, 2084.80848084809,
            2085.60856085609, 2086.40864086409, 2087.20872087209, 2088.00880088009
        };

        intensities = {
            0, 0, 0.298585462364539, 0.188908177104015,
            0, 0.380566263007272, 0.207680789399876, 0.110507439129649,
            0.0832567555737356, 0.165409332028748, 0.185605135114612, 0.219507662840845,
            0.380311951491821, 0.154004273557106, 0.378592237262633, 0.0589452781682507,
            0, 0.283578960805238, 0.243027948124485, 0.153480502037719,
            0.231274299584042, 0.565708003178639, 0.0845741327545553, 0.363814250712118,
            0.530574138576480, 0.450242351634906, 0.278465187341858, 0.360944309566526,
            0.536542189711439, 0.557343781971436, 0.261807240564701, 0.359553640289308,
            0.00295847497307244, 0.443706843804907, 0.436934799504090, 0.214428108181404,
            0, 0, 0, 0,
        };
        view = { DataView(mzs), DataView(intensities) };
    }

protected:
    Data mzs;
    Data intensities;
    SpectrumView view;
    static constexpr DataType MAX_ERROR = 0.0001;
};

TEST_F(SignalDecomposerTest, decomposes_splitter_segment_into_splitter)
{
    GmmOptions options;
    options.minComponentSearchIterations = 15;
    const DataType resolutionCoefficient = 0.00214166770575118;
    const Index splittingPeak = 29;

    MixtureModel components = DecomposeSplitterSegment(view, mzs[splittingPeak],
        resolutionCoefficient, options);
    MixtureModel validComponents = {
        /*       mean        std      weight        */
            { 2078.541294, 0.586043, 0.670626 },
            { 2079.549659, 0.443776, 0.537544 },
            { 2080.948794, 0.443776, 0.454383 },
    };
    ASSERT_EQ(validComponents.size(), components.size());
    for (Index i = 0; i < validComponents.size(); i++)
    {
        EXPECT_NEAR(components[i].mean, validComponents[i].mean, MAX_ERROR);
        EXPECT_NEAR(components[i].deviation, validComponents[i].deviation, MAX_ERROR);
        EXPECT_NEAR(components[i].weight, validComponents[i].weight, MAX_ERROR);
    }
}

TEST_F(SignalDecomposerTest, decomposes_signal_correctly)
{
    GmmOptions options;
    options.minComponentSearchIterations = 15;
    const DataType resolutionCoefficient = 0.00214166770575118;
    const DataType penaltyCoefficient = options.splittingPenaltyCoefficient;
    const int minNumOfBlocks = 2;
    const bool decomposingSplitter = true;

    MixtureModel components = DecomposeSignal(view, penaltyCoefficient,
        resolutionCoefficient, options, minNumOfBlocks, decomposingSplitter);
    MixtureModel validComponents = {
        /*       mean        std      weight        */
            { 2058.349253, 0.443776, 0.133505 },
            { 2058.349253, 0.443776, 0.257386 },
            { 2060.720187, 0.443776, 0.463778 },
            { 2062.085898, 0.443776, 0.101032 },
            { 2064.280051, 0.970573, 0.477677 },
            { 2065.965719, 0.443776, 0.393060 },
            { 2067.580994, 0.443776, 0.428431 },
            { 2070.412643, 0.443776, 0.413025 },
            { 2071.721076, 0.443776, 0.162176 },
            { 2073.106781, 0.443776, 0.656898 },
            { 2075.212363, 0.493164, 0.515861 },
            { 2076.430243, 0.749600, 0.810494 },
            { 2078.541294, 0.586043, 0.670626 },
            { 2079.549659, 0.443776, 0.537544 },
            { 2080.948794, 0.443776, 0.454383 },
            { 2083.548856, 0.443776, 0.598660 },
            { 2084.464778, 0.443776, 0.279677 },
    };
    ASSERT_EQ(validComponents.size(), components.size());
    for (Index i = 0; i < validComponents.size(); i++)
    {
        EXPECT_NEAR(components[i].mean, validComponents[i].mean, MAX_ERROR);
        EXPECT_NEAR(components[i].deviation, validComponents[i].deviation, MAX_ERROR);
        EXPECT_NEAR(components[i].weight, validComponents[i].weight, MAX_ERROR);
    }
}
}
