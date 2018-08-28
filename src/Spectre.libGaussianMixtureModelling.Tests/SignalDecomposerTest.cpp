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
#include "Spectre.libGaussianMixtureModelling/SegmentDecomposer.h"

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

    MixtureModel components = DecomposeSplitterSegment<DynProgInitialization,
    ExpectationMaximization<ExpectationRunnerOpt, MaximizationRunnerOpt>>(
        view, mzs[splittingPeak],
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

TEST_F(SignalDecomposerTest, decomposes_segment_into_gaussian_mixture_model)
{
    GmmOptions options;
    const DataType resolutionCoefficient = 0.00214166770575118;
    Spectrum spectrum;
    spectrum.mzs = {
        2004.00040004000, 2004.80048004800, 2005.60056005601, 2006.40064006401,
        2007.20072007201, 2008.00080008001, 2008.80088008801, 2009.60096009601,
        2010.40104010401, 2011.20112011201, 2012.00120012001, 2012.80128012801,
        2013.60136013601, 2014.40144014401, 2015.20152015202, 2016.00160016002,
        2016.80168016802, 2017.60176017602, 2018.40184018402, 2019.20192019202,
        2020.00200020002, 2020.80208020802, 2021.60216021602, 2022.40224022402,
        2023.20232023202, 2024.00240024002, 2024.80248024802, 2025.60256025603,
        2026.40264026403, 2027.20272027203, 2028.00280028003, 2028.80288028803,
        2029.60296029603, 2030.40304030403, 2031.20312031203, 2032.00320032003,
        2032.80328032803, 2033.60336033603, 2034.40344034403, 2035.20352035204,
    };
    spectrum.intensities = {
        0.00313896763865884, 0.471441652934461, 0.672030678788723,
        0.816262002801366, 1.08915090226750, 0.908525574332631, 1.09942659964331,
        1.00831471686859, 0.898924784053919, 0.830397142089201, 0.921254469136256,
        0.809914526511511, 0.495504714920023, 1.05933645531290, 0.715764166159622,
        0.654187646945404, 0.833199942407827, 0.472595554612829, 0.718948317424693,
        0.202560913964748, 0.512811386708187, 0.153311309685319, 0.793625363077548,
        0.435753748045594, 0.394305054377424, 0.535609833834569, 0.240742736542648,
        0.387432851938412, 0.581676305935098, 0.251998227512921, 0.437725964408568,
        0.299204203319242, 0.386115747151888, 0.0515198664285381, 0.444600759614117,
        0.0639361145978792, 0.342632162977885, 0.276130057228110, 0.207216215629398,
        0.159000891995082,
    };

    MixtureModel components =
        DecomposeSegment<DynProgInitialization, ExpectationMaximization<
        ExpectationRunnerOpt, MaximizationRunnerOpt>>(
            spectrum, resolutionCoefficient, options);

    MixtureModel validComponents = {
        /*   mean        std      weight  */
        { 2005.263849, 0.438133, 0.855414 },
        { 2006.429610, 0.432532, 0.692093 },
        { 2007.481088, 0.549456, 1.488316 },
        { 2009.110685, 0.603146, 1.779638 },
        { 2010.738726, 0.613304, 1.384398 },
        { 2012.383269, 0.607698, 1.507187 },
        { 2014.526220, 0.628876, 1.685200 },
        { 2016.621240, 0.707594, 1.522562 },
        { 2018.369222, 0.432532, 0.745471 },
        { 2019.989147, 0.432532, 0.554604 },
        { 2021.794029, 0.442122, 0.967499 },
        { 2023.787760, 0.716577, 0.991234 },
        { 2026.222886, 0.611643, 0.931179 },
        { 2027.987672, 0.432532, 0.463956 },
        { 2029.318047, 0.432532, 0.502840 },
        { 2031.224134, 0.432532, 0.430288 },
        { 2033.150348, 0.442025, 0.501883 },
        { 2034.729878, 0.432532, 0.293676 },
    };
    ASSERT_EQ(validComponents.size(), components.size());
    for (Index i = 0; i < validComponents.size(); i++)
    {
        EXPECT_NEAR(components[i].mean, validComponents[i].mean, MAX_ERROR);
        EXPECT_NEAR(components[i].deviation, validComponents[i].deviation, MAX_ERROR);
        EXPECT_NEAR(components[i].weight, validComponents[i].weight, MAX_ERROR);
    }
}

TEST_F(SignalDecomposerTest, decomposes_segment_in_special_case)
{
    GmmOptions options;
    const DataType resolutionCoefficient = 0.00214166770575118;
    Spectrum spectrum;
    spectrum.mzs = {
        2178.41784178418, 2179.21792179218, 2180.01800180018, 2180.81808180818,
        2182.41824182418, 2183.21832183218, 2184.81848184819, 2187.21872187219,
        2188.81888188819, 2190.41904190419, 2195.21952195220, 2199.21992199220,
        2200.02000200020, 2203.22032203220, 2205.62056205621, 2207.22072207221,
        2208.02080208021, 2208.82088208821, 2209.62096209621, 2210.42104210421,
        2211.22112211221, 2212.02120212021, 2212.82128212821, 2213.62136213621,
        2214.42144214421, 2215.22152215222, 2216.02160216022, 2216.82168216822,
        2217.62176217622, 2218.42184218422, 2219.22192219222, 2220.02200220022,
        2220.82208220822, 2221.62216221622, 2222.42224222422, 2223.22232223222,
        2224.02240224022, 2224.82248224823, 2225.62256225623, 2226.42264226423,
    };
    spectrum.intensities = {
        0.0104103037322122, 0.108274153131202, 0.0431182962200357, 0.0626828670440743,
        0.00330516550267927, 0.0186287726733519, 0.0948714627159854, 0.0602589452346652,
        0.137531148228755, 0.0938529704075883, 0.285868418820212, 0.00586812472444395,
        0.0723354969826420, 0.158728966716669, 0.160908548692869, 0.339596285562561,
        0.158684208342835, 0.748643744562148, 0.671838470020093, 1.51516821723200,
        1.73394713274720, 2.89783527200036, 4.32616388020944, 5.87393933454299,
        7.89008760478228, 10.5030513540448, 13.2502610806335, 15.4586656269736,
        18.2466568070642, 21.2409482849845, 21.9424679435353, 22.9060290139104,
        22.9708090159931, 22.1597044060178, 20.6691040458441, 18.4386172461547,
        15.3653103083655, 13.8527580883598, 12.0420210964656, 8.85893145174516,
    };

    MixtureModel components =
        DecomposeSegment<DynProgInitialization, ExpectationMaximization<
        ExpectationRunnerOpt, MaximizationRunnerOpt>>(spectrum,
            resolutionCoefficient, options);
    // TODO figure out why do these components slightly differ
    MixtureModel validComponents = {
        /*       mean               std                 weight        */
        { 2191.8622063823000, 8.6751259603320072, 0.96882519631418773 },
        { 2214.6675006561072, 3.2223749101535879, 22.959185704352869 },
        { 2217.3663913301739, 2.3118103096776972, 68.208778550227933 },
        { 2220.4501650426491, 1.7576054413770283, 73.791943622169171 },
        { 2223.0120062729125, 1.1201534516883531, 36.404250897403514 },
        { 2224.6173940098060, 0.47255483473253318, 11.406874305406063 },
        { 2225.9460295053218, 0.47255483473253318, 15.630955723353708 },
    };
    ASSERT_EQ(validComponents.size(), components.size());
    for (Index i = 0; i < validComponents.size(); i++)
    {
        EXPECT_NEAR(components[i].mean, validComponents[i].mean, MAX_ERROR);
        EXPECT_NEAR(components[i].deviation, validComponents[i].deviation, MAX_ERROR);
        EXPECT_NEAR(components[i].weight, validComponents[i].weight, MAX_ERROR);
    }
}

TEST_F(SignalDecomposerTest, returns_empty_gmm_on_small_signal)
{
    GmmOptions options;
    const DataType resolutionCoefficient = 0.0;
    Spectrum signal;
    signal.mzs         = Data(2);
    signal.intensities = Data(2);
    EXPECT_EQ(0, (DecomposeSegment<DynProgInitialization,
        ExpectationMaximization<ExpectationRunnerOpt, MaximizationRunnerOpt>>(
            signal, resolutionCoefficient, options).size()));
}

TEST_F(SignalDecomposerTest, decomposes_signal_correctly)
{
    GmmOptions options;
    options.minComponentSearchIterations = 15;
    const DataType resolutionCoefficient = 0.00214166770575118;
    const DataType penaltyCoefficient = options.splittingPenaltyCoefficient;
    const int minNumOfBlocks = 2;
    const bool decomposingSplitter = true;

    MixtureModel components = DecomposeSignal<DynProgInitialization, ExpectationMaximization<
        ExpectationRunnerOpt, MaximizationRunnerOpt>>(view, penaltyCoefficient,
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
