add_executable(Spectre.libPeakFinder.Tests
        ExtremaFinderTest.cpp
        FWHHCalculatorTest.cpp
        PeakQualityCalculatorTest.cpp
        )

target_link_libraries(Spectre.libPeakFinder.Tests
        Spectre.libPeakFinder
        Spectre.libException
        gmock_main
        )

add_test(Spectre.libPeakFinder.Tests Spectre.libPeakFinder.Tests)
