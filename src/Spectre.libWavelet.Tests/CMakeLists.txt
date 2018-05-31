add_executable(Spectre.libWavelet.Tests
        ConvolutionTest.cpp
        DaubechiesFiltersDenoiserTest.cpp
        FloatingPointVectorMatcher.h
        MedianAbsoluteDeviationNoiseEstimatorTest.cpp
        SoftThresholderTest.cpp
        WaveletDecomposerRefTest.cpp
        WaveletReconstructorRefTest.cpp
        )

target_link_libraries(Spectre.libWavelet.Tests
        Spectre.libWavelet
        Spectre.libException
        Spectre.libFunctional
        gtest_main
        )

add_test(Spectre.libWavelet.Tests Spectre.libWavelet.Tests)
