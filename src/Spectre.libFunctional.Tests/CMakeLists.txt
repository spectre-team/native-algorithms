add_executable(Spectre.libFunctional.Tests
        FilterTest.cpp
        FindTest.cpp
        RangeTest.cpp
        TransformTest.cpp
        )

target_link_libraries(Spectre.libFunctional.Tests
        Spectre.libFunctional
        Spectre.libException
        gtest_main
        )

add_test(Spectre.libFunctional.Tests Spectre.libFunctional.Tests)
