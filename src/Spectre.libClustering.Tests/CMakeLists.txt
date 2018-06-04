add_executable(Spectre.libClustering.Tests
        PartitionTest.cpp
        )

target_link_libraries(Spectre.libClustering.Tests
        Spectre.libClustering
        Spectre.libException
        gtest_main
        )

add_test(Spectre.libClustering.Tests Spectre.libClustering.Tests)
