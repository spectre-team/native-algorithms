cmake_minimum_required(VERSION 3.0)
project(spectre_native_algorithms CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/finders")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/configs")

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED core ml)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if (result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if (result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories(BEFORE SYSTEM
  "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

include(CheckIncludeFileCXX)
check_include_file_cxx(/usr/include/gsl/gsl HAVE_GSL)
if (NOT HAVE_GSL)
    include_directories(dep/gsl/include)
    message(STATUS "Using GSL includes from dep/")
endif ()

include_directories(src)


add_library(Spectre.libClassifier STATIC
        src/Spectre.libClassifier/ConfusionMatrix.cpp
        src/Spectre.libClassifier/ConfusionMatrix.h
        src/Spectre.libClassifier/DatasetFilter.h
        src/Spectre.libClassifier/DownsampledOpenCVDataset.cpp
        src/Spectre.libClassifier/DownsampledOpenCVDataset.h
        src/Spectre.libClassifier/EmptyOpenCvDatasetException.cpp
        src/Spectre.libClassifier/EmptyOpenCvDatasetException.h
        src/Spectre.libClassifier/ExcessiveTrainingRateException.cpp
        src/Spectre.libClassifier/ExcessiveTrainingRateException.h
        src/Spectre.libClassifier/IClassifier.h
        src/Spectre.libClassifier/InsufficientDataException.cpp
        src/Spectre.libClassifier/InsufficientDataException.h
        src/Spectre.libClassifier/NegativeTrainingRateException.cpp
        src/Spectre.libClassifier/NegativeTrainingRateException.h
        src/Spectre.libClassifier/NotABinaryLabelException.cpp
        src/Spectre.libClassifier/NotABinaryLabelException.h
        src/Spectre.libClassifier/ObservationExtractor.cpp
        src/Spectre.libClassifier/ObservationExtractor.h
        src/Spectre.libClassifier/OpenCvDataset.cpp
        src/Spectre.libClassifier/OpenCvDataset.h
        src/Spectre.libClassifier/RandomSplitter.cpp
        src/Spectre.libClassifier/RandomSplitter.h
        src/Spectre.libClassifier/SplittedOpenCvDataset.cpp
        src/Spectre.libClassifier/SplittedOpevCvDataset.h
        src/Spectre.libClassifier/Svm.cpp
        src/Spectre.libClassifier/Types.h
        src/Spectre.libClassifier/UnsupportedDatasetTypeException.h
        src/Spectre.libClassifier/UntrainedClassifierException.h
        )


add_library(Spectre.libClustering STATIC
        src/Spectre.libClustering/Partition.cpp
        src/Spectre.libClustering/Partition.h
        )


add_library(Spectre.libDataset STATIC
        src/Spectre.libDataset/Dataset.h
        src/Spectre.libDataset/Empty.cpp
        src/Spectre.libDataset/Empty.h
        src/Spectre.libDataset/IDataset.h
        src/Spectre.libDataset/InconsistentInputSizeException.cpp
        src/Spectre.libDataset/InconsistentInputSizeException.h
        src/Spectre.libDataset/IReadOnlyDataset.h
        )


add_library(Spectre.libDivik STATIC
        src/Spectre.libDivik/DivikResultStruct.cpp
        src/Spectre.libDivik/DivikResultStruct.h
        )


add_library(Spectre.libException STATIC
        src/Spectre.libException/ArgumentOutOfRangeException.h
        src/Spectre.libException/EmptyArgumentException.cpp
        src/Spectre.libException/EmptyArgumentException.h
        src/Spectre.libException/EmptyDatasetException.cpp
        src/Spectre.libException/EmptyDatasetException.h
        src/Spectre.libException/ExceptionBase.cpp
        src/Spectre.libException/ExceptionBase.h
        src/Spectre.libException/InconsistentArgumentSizesException.cpp
        src/Spectre.libException/InconsistentArgumentSizesException.h
        src/Spectre.libException/NullPointerException.cpp
        src/Spectre.libException/NullPointerException.h
        src/Spectre.libException/OutOfRangeException.cpp
        src/Spectre.libException/OutOfRangeException.h
        src/Spectre.libException/ReachedUnreachableCodeException.cpp
        src/Spectre.libException/ReachedUnreachableCodeException.h
        )


add_library(Spectre.libFunctional STATIC
        src/Spectre.libFunctional/Filter.h
        src/Spectre.libFunctional/Find.cpp
        src/Spectre.libFunctional/Find.h
        src/Spectre.libFunctional/Range.h
        src/Spectre.libFunctional/Transform.h
        src/Spectre.libFunctional/ZeroStepException.cpp
        src/Spectre.libFunctional/ZeroStepException.h
        )


add_library(Spectre.libGaClassifier STATIC
        src/Spectre.libGaClassifier/AllLabelTypesIncludedCondition.cpp
        src/Spectre.libGaClassifier/AllLabelTypesIncludedCondition.h
        src/Spectre.libGaClassifier/ClassifierFitnessFunction.cpp
        src/Spectre.libGaClassifier/ClassifierFitnessFunction.h
        src/Spectre.libGaClassifier/GaClassifier.cpp
        src/Spectre.libGaClassifier/GaClassifier.h
        src/Spectre.libGaClassifier/IndividualFeasibilityConditionsFactory.cpp
        src/Spectre.libGaClassifier/IndividualFeasibilityConditionsFactory.h
        )


add_library(Spectre.libGaussianMixtureModelling STATIC
        src/Common/DataTypes.h
        src/Spectre.libGaussianMixtureModelling/ClearPeakFinder.cpp
        src/Spectre.libGaussianMixtureModelling/ClearPeakFinder.h
        src/Spectre.libGaussianMixtureModelling/DataTypes.h
        src/Spectre.libGaussianMixtureModelling/DynamicProgrammingInitialization.h
        src/Spectre.libGaussianMixtureModelling/ExpectationMaximization.cpp
        src/Spectre.libGaussianMixtureModelling/ExpectationMaximization.h
        src/Spectre.libGaussianMixtureModelling/ExpectationRunnerRef.h
        src/Spectre.libGaussianMixtureModelling/GaussianDistribution.h
        src/Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h
        src/Spectre.libGaussianMixtureModelling/GmmOptions.h
        src/Spectre.libGaussianMixtureModelling/LogLikelihoodCalculator.h
        src/Spectre.libGaussianMixtureModelling/Matrix.h
        src/Spectre.libGaussianMixtureModelling/MaximizationRunnerRef.h
        src/Spectre.libGaussianMixtureModelling/RandomInitializationRef.h
        src/Spectre.libGaussianMixtureModelling/SplitterSegmentExtractor.cpp
        src/Spectre.libGaussianMixtureModelling/SplitterSegmentExtractor.h
        )


add_library(Spectre.libGenetic STATIC
        src/Spectre.libGenetic/BaseIndividualFeasibilityCondition.cpp
        src/Spectre.libGenetic/BaseIndividualFeasibilityCondition.h
        src/Spectre.libGenetic/CrossoverOperator.cpp
        src/Spectre.libGenetic/CrossoverOperator.h
        src/Spectre.libGenetic/DataTypes.h
        src/Spectre.libGenetic/ExcessivePreservationRateException.cpp
        src/Spectre.libGenetic/ExcessivePreservationRateException.h
        src/Spectre.libGenetic/FitnessFunction.h
        src/Spectre.libGenetic/Generation.cpp
        src/Spectre.libGenetic/Generation.h
        src/Spectre.libGenetic/GenerationFactory.cpp
        src/Spectre.libGenetic/GenerationFactory.h
        src/Spectre.libGenetic/GeneticAlgorithm.cpp
        src/Spectre.libGenetic/GeneticAlgorithm.h
        src/Spectre.libGenetic/GeneticAlgorithmFactory.cpp
        src/Spectre.libGenetic/GeneticAlgorithmFactory.h
        src/Spectre.libGenetic/InconsistentChromosomeLengthException.cpp
        src/Spectre.libGenetic/InconsistentChromosomeLengthException.h
        src/Spectre.libGenetic/InconsistentGenerationAndScoresLengthException.cpp
        src/Spectre.libGenetic/InconsistentGenerationAndScoresLengthException.h
        src/Spectre.libGenetic/InconsistentMinimalAndMaximalFillupException.cpp
        src/Spectre.libGenetic/InconsistentMinimalAndMaximalFillupException.h
        src/Spectre.libGenetic/Individual.cpp
        src/Spectre.libGenetic/Individual.h
        src/Spectre.libGenetic/IndividualsBuilderStrategy.cpp
        src/Spectre.libGenetic/IndividualsBuilderStrategy.h
        src/Spectre.libGenetic/LengthCondition.cpp
        src/Spectre.libGenetic/LengthCondition.h
        src/Spectre.libGenetic/MaximalFillupCondition.cpp
        src/Spectre.libGenetic/MaximalFillupCondition.h
        src/Spectre.libGenetic/MaximalPercentageFillupCondition.cpp
        src/Spectre.libGenetic/MaximalPercentageFillupCondition.h
        src/Spectre.libGenetic/MinimalFillupCondition.cpp
        src/Spectre.libGenetic/MinimalFillupCondition.h
        src/Spectre.libGenetic/MinimalPercentageFillupCondition.cpp
        src/Spectre.libGenetic/MinimalPercentageFillupCondition.h
        src/Spectre.libGenetic/MutationOperator.cpp
        src/Spectre.libGenetic/MutationOperator.h
        src/Spectre.libGenetic/NegativePreservationRateException.cpp
        src/Spectre.libGenetic/NegativePreservationRateException.h
        src/Spectre.libGenetic/OffspringGenerator.cpp
        src/Spectre.libGenetic/OffspringGenerator.h
        src/Spectre.libGenetic/ParentSelectionStrategy.cpp
        src/Spectre.libGenetic/ParentSelectionStrategy.h
        src/Spectre.libGenetic/PreservationStrategy.cpp
        src/Spectre.libGenetic/PreservationStrategy.h
        src/Spectre.libGenetic/Scorer.cpp
        src/Spectre.libGenetic/Scorer.h
        src/Spectre.libGenetic/Sorting.h
        src/Spectre.libGenetic/StopCondition.cpp
        src/Spectre.libGenetic/StopCondition.h
        )


add_library(Spectre.libHeatmapDataScaling STATIC
        src/Spectre.libHeatmapDataScaling/HeatmapDataScalingAlgorithm.h
        src/Spectre.libHeatmapDataScaling/Normalization.cpp
        src/Spectre.libHeatmapDataScaling/Normalization.h
        )


add_library(Spectre.libPeakFinder STATIC
        src/Common/DataTypes.h
        src/Spectre.libPeakFinder/ExtremaFinder.cpp
        src/Spectre.libPeakFinder/ExtremaFinder.h
        src/Spectre.libPeakFinder/FWHHCalculator.cpp
        src/Spectre.libPeakFinder/FWHHCalculator.h
        src/Spectre.libPeakFinder/PeakFinder.cpp
        src/Spectre.libPeakFinder/PeakFinder.h
        src/Spectre.libPeakFinder/PeakQualityCalculator.cpp
        src/Spectre.libPeakFinder/PeakQualityCalculator.h
        )


add_library(Spectre.libStatistics STATIC
        src/Spectre.libStatistics/CohenEffectSize.cpp
        src/Spectre.libStatistics/CohenEffectSize.h
        src/Spectre.libStatistics/DifferentiatingFeaturesEstimator.cpp
        src/Spectre.libStatistics/DifferentiatingFeaturesEstimator.h
        src/Spectre.libStatistics/InconsistentNumberOfFeaturesException.cpp
        src/Spectre.libStatistics/InconsistentNumberOfFeaturesException.h
        src/Spectre.libStatistics/Math.h
        src/Spectre.libStatistics/StatisticalIndex.cpp
        src/Spectre.libStatistics/StatisticalIndex.h
        src/Spectre.libStatistics/Statistics.h
        src/Spectre.libStatistics/Types.h
        src/Spectre.libStatistics/ValuesHomogeneityEstimator.h
        )


add_library(Spectre.libWavelet STATIC
        src/Common/DataTypes.h
        src/Spectre.libWavelet/AlgorithmConstants.h
        src/Spectre.libWavelet/Convolution.cpp
        src/Spectre.libWavelet/Convolution.h
        src/Spectre.libWavelet/DataTypes.h
        src/Spectre.libWavelet/DaubechiesFiltersDenoiser.cpp
        src/Spectre.libWavelet/DaubechiesFiltersDenoiser.h
        src/Spectre.libWavelet/MedianAbsoluteDeviationNoiseEstimator.cpp
        src/Spectre.libWavelet/MedianAbsoluteDeviationNoiseEstimator.h
        src/Spectre.libWavelet/PrecomputedDaubechiesCoefficients.h
        src/Spectre.libWavelet/SoftThresholder.cpp
        src/Spectre.libWavelet/SoftThresholder.h
        src/Spectre.libWavelet/WaveletCoefficients.h
        src/Spectre.libWavelet/WaveletDecomposerRef.cpp
        src/Spectre.libWavelet/WaveletDecomposerRef.h
        src/Spectre.libWavelet/WaveletReconstructorRef.cpp
        src/Spectre.libWavelet/WaveletReconstructorRef.h
        src/Spectre.libWavelet/WaveletUtils.h
        )


add_executable(Spectre.libClassifier.Tests
        src/Spectre.libClassifier.Tests/DownsampledOpenCVDatasetTest.cpp
        src/Spectre.libClassifier.Tests/ObservationExtractorTest.cpp
        src/Spectre.libClassifier.Tests/OpenCvDatasetTest.cpp
        src/Spectre.libClassifier.Tests/RandomSplitterTest.cpp
        src/Spectre.libClassifier.Tests/SplittedOpenCVDatasetTest.cpp
        src/Spectre.libClassifier.Tests/SvmTest.cpp
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


add_executable(Spectre.libClustering.Tests
        src/Spectre.libClustering.Tests/PartitionTest.cpp
        )
target_link_libraries(Spectre.libClustering.Tests
        Spectre.libClustering
        Spectre.libException
        gtest_main
        )
add_test(Spectre.libClustering.Tests Spectre.libClustering.Tests)


add_executable(Spectre.libDataset.Tests
        src/Spectre.libDataset.Tests/DatasetTest.cpp
        )
target_link_libraries(Spectre.libDataset.Tests
        Spectre.libDataset
        Spectre.libException
        gtest_main
        )
add_test(Spectre.libDataset.Tests Spectre.libDataset.Tests)


add_executable(Spectre.libFunctional.Tests
        src/Spectre.libFunctional.Tests/FilterTest.cpp
        src/Spectre.libFunctional.Tests/FindTest.cpp
        src/Spectre.libFunctional.Tests/RangeTest.cpp
        src/Spectre.libFunctional.Tests/TransformTest.cpp
        )
target_link_libraries(Spectre.libFunctional.Tests
        Spectre.libFunctional
        Spectre.libException
        gtest_main
        )
add_test(Spectre.libFunctional.Tests Spectre.libFunctional.Tests)


add_executable(Spectre.libGaClassifier.Tests
        src/Spectre.libGaClassifier.Tests/AllLabelTypesIncludedConditionTest.cpp
        src/Spectre.libGaClassifier.Tests/ClassifierFitnessFunctionTest.cpp
        src/Spectre.libGaClassifier.Tests/GaClassifierTest.cpp
        src/Spectre.libGaClassifier.Tests/IndividualFeasibilityConditionsFactoryTest.cpp
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


add_executable(Spectre.libGaussianMixtureModelling.Tests
        src/Spectre.libGaussianMixtureModelling.Tests/ClearPeakFinderTest.cpp
        src/Spectre.libGaussianMixtureModelling.Tests/DynamicProgramminInitializationTest.cpp
        src/Spectre.libGaussianMixtureModelling.Tests/ExpectationMaximizationTest.cpp
        src/Spectre.libGaussianMixtureModelling.Tests/GaussianMixtureModelTest.cpp
        src/Spectre.libGaussianMixtureModelling.Tests/SplitterSegmentExtractorTest.cpp
        )
target_link_libraries(Spectre.libGaussianMixtureModelling.Tests
        Spectre.libGaussianMixtureModelling
        Spectre.libException
        gtest_main
        )
add_test(Spectre.libGaussianMixtureModelling.Tests Spectre.libGaussianMixtureModelling.Tests)


add_executable(Spectre.libGenetic.Tests
        src/Spectre.libGenetic.Tests/BaseIndividualFeasibilityConditionTest.cpp
        src/Spectre.libGenetic.Tests/CrossoverOperatorTest.cpp
        src/Spectre.libGenetic.Tests/GenerationFactoryTest.cpp
        src/Spectre.libGenetic.Tests/GenerationTest.cpp
        src/Spectre.libGenetic.Tests/GeneticAlgorithmTest.cpp
        src/Spectre.libGenetic.Tests/IndividualsBuilderStrategyTest.cpp
        src/Spectre.libGenetic.Tests/IndividualTest.cpp
        src/Spectre.libGenetic.Tests/LengthConditionTest.cpp
        src/Spectre.libGenetic.Tests/MaximalFillupConditionTest.cpp
        src/Spectre.libGenetic.Tests/MaximalPercentageFillupConditionTest.cpp
        src/Spectre.libGenetic.Tests/MinimalFillupConditionTest.cpp
        src/Spectre.libGenetic.Tests/MinimalPercentageFillupConditionTest.cpp
        src/Spectre.libGenetic.Tests/MockBaseIndividualFeasibilityCondition.h
        src/Spectre.libGenetic.Tests/MockCrossoverOperator.h
        src/Spectre.libGenetic.Tests/MockFitnessFunction.h
        src/Spectre.libGenetic.Tests/MockIndividualsBuilderStrategy.h
        src/Spectre.libGenetic.Tests/MockMutationOperator.h
        src/Spectre.libGenetic.Tests/MockOffspringGenerator.h
        src/Spectre.libGenetic.Tests/MockParentSelectionStrategy.h
        src/Spectre.libGenetic.Tests/MockPreservationStrategy.h
        src/Spectre.libGenetic.Tests/MockScorer.h
        src/Spectre.libGenetic.Tests/MockStopCondition.h
        src/Spectre.libGenetic.Tests/MutationOperatorTest.cpp
        src/Spectre.libGenetic.Tests/OffspringGeneratorTest.cpp
        src/Spectre.libGenetic.Tests/ParentSelectionStrategyTest.cpp
        src/Spectre.libGenetic.Tests/PreservationStrategyTest.cpp
        src/Spectre.libGenetic.Tests/ScorerTest.cpp
        src/Spectre.libGenetic.Tests/StopConditionTest.cpp
        )
target_link_libraries(Spectre.libGenetic.Tests
        Spectre.libGenetic
        Spectre.libException
        ${OpenMP_CXX_FLAGS}
        gmock_main
        )
add_test(Spectre.libGenetic.Tests Spectre.libGenetic.Tests)


add_executable(Spectre.libPeakFinder.Tests
        src/Spectre.libPeakFinder.Tests/ExtremaFinderTest.cpp
        src/Spectre.libPeakFinder.Tests/FWHHCalculatorTest.cpp
        src/Spectre.libPeakFinder.Tests/PeakQualityCalculatorTest.cpp
        )
target_link_libraries(Spectre.libPeakFinder.Tests
        Spectre.libPeakFinder
        Spectre.libException
        gmock_main
        )
add_test(Spectre.libPeakFinder.Tests Spectre.libPeakFinder.Tests)


add_executable(Spectre.libStatistics.Tests
        src/Spectre.libStatistics.Tests/CohenEffectSizeTest.cpp
        src/Spectre.libStatistics.Tests/DifferentiatingFeaturesEstimatorTest.cpp
        src/Spectre.libStatistics.Tests/MathTest.cpp
        src/Spectre.libStatistics.Tests/StatisticalIndexTest.cpp
        src/Spectre.libStatistics.Tests/StatisticsTest.cpp
        src/Spectre.libStatistics.Tests/ValuesHomogeneityEstimatorMock.h
        )
target_link_libraries(Spectre.libStatistics.Tests
        Spectre.libStatistics
        Spectre.libDataset
        Spectre.libException
        ${OpenMP_CXX_FLAGS}
        gmock_main
        )
add_test(Spectre.libStatistics.Tests Spectre.libStatistics.Tests)


add_executable(Spectre.libWavelet.Tests
        src/Spectre.libWavelet.Tests/ConvolutionTest.cpp
        src/Spectre.libWavelet.Tests/DaubechiesFiltersDenoiserTest.cpp
        src/Spectre.libWavelet.Tests/FloatingPointVectorMatcher.h
        src/Spectre.libWavelet.Tests/MedianAbsoluteDeviationNoiseEstimatorTest.cpp
        src/Spectre.libWavelet.Tests/SoftThresholderTest.cpp
        src/Spectre.libWavelet.Tests/WaveletDecomposerRefTest.cpp
        src/Spectre.libWavelet.Tests/WaveletReconstructorRefTest.cpp
        )
target_link_libraries(Spectre.libWavelet.Tests
        Spectre.libWavelet
        Spectre.libException
        Spectre.libFunctional
        gtest_main
        )
add_test(Spectre.libWavelet.Tests Spectre.libWavelet.Tests)


enable_testing()
