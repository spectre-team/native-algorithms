add_executable(Spectre.libGaussianMixtureModelling.Tests
        ClearPeakFinderTest.cpp
        DynamicProgramminInitializationTest.cpp
        ExpectationMaximizationTest.cpp
        GaussianDistributionTest.cpp
        GaussianMixtureModelTest.cpp
        SignalDecomposerTest.cpp
        SplitterSegmentExtractorTest.cpp
        )

target_link_libraries(Spectre.libGaussianMixtureModelling.Tests
        Spectre.libGaussianMixtureModelling
	Spectre.libFunctional
        Spectre.libException
        Spectre.libStatistics
        gtest_main
        )

add_test(Spectre.libGaussianMixtureModelling.Tests Spectre.libGaussianMixtureModelling.Tests)
