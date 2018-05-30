add_library(Spectre.libGaussianMixtureModelling STATIC
        ClearPeakFinder.cpp
        ClearPeakFinder.h
        DataTypes.h
        DynamicProgrammingInitialization.h
        ExpectationMaximization.cpp
        ExpectationMaximization.h
        ExpectationRunnerRef.h
        GaussianDistribution.h
        GaussianMixtureModel.h
        GmmOptions.h
        LogLikelihoodCalculator.h
        Matrix.h
        MaximizationRunnerRef.h
        RandomInitializationRef.h
        SplitterSegmentExtractor.cpp
        SplitterSegmentExtractor.h
        )
