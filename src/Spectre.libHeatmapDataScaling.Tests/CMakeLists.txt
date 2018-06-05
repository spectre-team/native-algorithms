add_executable(Spectre.libHeatmapDataScaling.Tests
        HistogramEqualizationTest.cpp
        ScalingDataUtilsTest.cpp
        SuppressionAlgorithmTest.cpp
        )

target_link_libraries(Spectre.libHeatmapDataScaling.Tests
        Spectre.libHeatmapDataScaling
        Spectre.libException
        gmock_main
        )

add_test(Spectre.libHeatmapDataScaling.Tests Spectre.libHeatmapDataScaling.Tests)
