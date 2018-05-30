add_executable(Spectre.libGaClassifier.Tests
        AllLabelTypesIncludedConditionTest.cpp
        ClassifierFitnessFunctionTest.cpp
        GaClassifierTest.cpp
        IndividualFeasibilityConditionsFactoryTest.cpp
        )

target_link_libraries(Spectre.libGaClassifier.Tests
        Spectre.libGaClassifier
        Spectre.libGenetic
        Spectre.libClassifier
        Spectre.libDataset
        Spectre.libException
        Spectre.libFunctional
        ${OpenCV_LIBS}
        ${OpenMP_CXX_FLAGS}
        gtest_main
        )

add_test(Spectre.libGaClassifier.Tests Spectre.libGaClassifier.Tests)
