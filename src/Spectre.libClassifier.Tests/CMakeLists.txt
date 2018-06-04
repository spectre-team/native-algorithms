add_executable(Spectre.libClassifier.Tests
        DatasetFactoryTest.cpp
        DownsampledOpenCVDatasetTest.cpp
        ObservationExtractorTest.cpp
        OpenCvDatasetTest.cpp
        RandomSplitterTest.cpp
        SplittedOpenCVDatasetTest.cpp
        SvmTest.cpp
        )

target_link_libraries(Spectre.libClassifier.Tests
        Spectre.libClassifier
        Spectre.libDataset
        Spectre.libException
        Spectre.libFunctional
        ${OpenCV_LIBS}
        gtest_main
        )

add_test(Spectre.libClassifier.Tests Spectre.libClassifier.Tests)
