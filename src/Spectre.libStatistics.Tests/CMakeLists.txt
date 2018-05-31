add_executable(Spectre.libStatistics.Tests
        CohenEffectSizeTest.cpp
        DifferentiatingFeaturesEstimatorTest.cpp
        MathTest.cpp
        StatisticalIndexTest.cpp
        StatisticsTest.cpp
        ValuesHomogeneityEstimatorMock.h
        )

target_link_libraries(Spectre.libStatistics.Tests
        Spectre.libStatistics
        Spectre.libDataset
        Spectre.libException
        ${OpenMP_CXX_FLAGS}
        gmock_main
        )

add_test(Spectre.libStatistics.Tests Spectre.libStatistics.Tests)
