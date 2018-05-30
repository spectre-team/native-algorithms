add_executable(Spectre.libDataset.Tests
        DatasetTest.cpp
        )

target_link_libraries(Spectre.libDataset.Tests
        Spectre.libDataset
        Spectre.libException
        gtest_main
        )

add_test(Spectre.libDataset.Tests Spectre.libDataset.Tests)
