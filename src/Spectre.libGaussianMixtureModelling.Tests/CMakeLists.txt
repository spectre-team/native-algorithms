add_executable(Spectre.libGaussianMixtureModelling.Tests
        ClearPeakFinderTest.cpp
        DynamicProgramminInitializationTest.cpp
        ExpectationMaximizationTest.cpp
        GaussianMixtureModelTest.cpp
        SplitterSegmentExtractorTest.cpp
        )

target_link_libraries(Spectre.libGaussianMixtureModelling.Tests
        Spectre.libGaussianMixtureModelling
        Spectre.libException
        gtest_main
        )

add_test(Spectre.libGaussianMixtureModelling.Tests Spectre.libGaussianMixtureModelling.Tests)
