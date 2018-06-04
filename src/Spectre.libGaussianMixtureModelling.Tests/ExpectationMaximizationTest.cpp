/*
* ExpectationMaximizationTest.cpp
* Provides implementation of tests for the purpose of testing
* Gaussian Mixture Modelling Expectation Maximization algorithm.
*
Copyright 2017 Michal Gallus

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#define GTEST_LANG_CXX11 1

#include <gtest/gtest.h>
#include "Spectre.libGaussianMixtureModelling/ExpectationMaximization.h"
#include "Spectre.libGaussianMixtureModelling/RandomInitializationRef.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationRunnerRef.h"
#include "Spectre.libGaussianMixtureModelling/MaximizationRunnerRef.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/MaximizationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/LogLikelihoodCalculator.h"

typedef std::mt19937_64 RandomNumberGenerator;

namespace spectre::unsupervised::gmm
{
class ExpectationMaximizationTest : public ::testing::Test
{
protected:

    Spectrum spectrum;
    std::vector<GaussianComponent> gaussianComponents;

    virtual void SetUp() override
    {
        gaussianComponents = {
            { /*mean =*/-5.0, /*deviation =*/ 3.0, /*weight =*/ 0.25 },
            { /*mean =*/ 5.0, /*deviation =*/ 3.0, /*weight =*/ 0.25 },
            { /*mean =*/-2.0, /*deviation =*/ 9.0, /*weight =*/ 0.5 }
        };

        spectrum.mzs = GenerateRange(-20.0, 20.0, 0.5);
        spectrum.intensities = GenerateValues(spectrum.mzs, gaussianComponents);
    }

    Data GenerateRange(DataType start, DataType end, DataType step)
    {
        const int size = int((end - start) / step);
        Data result(size, start);

        for (int i = 0; i < size; i++)
        {
            result[i] += step * i;
        }

        return result;
    }

    Data GenerateValues(Data input, const std::vector<GaussianComponent> &components)
    {
        Data output(input.size(), 0.0);
        for (unsigned i = 0; i < input.size(); i++)
        {
            for (unsigned j = 0; j < components.size(); j++)
            {
                output[i] += components[j].weight *
                    Gaussian(input[i], components[j].mean, components[j].deviation);
            }
        }
        return output;
    }
};

// Integration test
TEST_F(ExpectationMaximizationTest, test_whole_em)
{
    RandomNumberGenerator rngEngine(0);

    GaussianMixtureModel model(3);
    ExpectationMaximization<
        ExpectationRunnerRef,
        MaximizationRunnerRef>
    em(SpectrumView(spectrum), model);

    RandomInitializationRef Initializer =
        RandomInitializationRef(spectrum.mzs, model, rngEngine);
    Initializer.AssignRandomMeans();
    Initializer.AssignVariances();
    Initializer.AssignWeights();

    GaussianMixtureModel result = em.EstimateGmm(0.4635, 0.001);
    Data approximates = GenerateValues(spectrum.mzs, result);

    Data errors(spectrum.intensities.size());
    for (unsigned i = 0; i < spectrum.intensities.size(); i++)
    {
        errors[i] = pow(spectrum.intensities[i] - approximates[i], 2) * 10000.0; // scale the errors
    }

    DataType averageError = 0.0;
    for (unsigned i = 0; i < errors.size(); i++)
    {
        averageError += errors[i];
    }
    averageError /= errors.size();
    DataType maxError = *std::max_element(errors.begin(), errors.end());
    DataType minError = *std::min_element(errors.begin(), errors.end());
    printf("Avg error: %f\n", averageError);
    printf("Max error: %f\n", maxError);
    printf("Min error: %f\n", minError);

    EXPECT_LE(averageError, 0.1);
}

TEST_F(ExpectationMaximizationTest, test_em_ref_initialization)
{
    RandomNumberGenerator rngEngine(0);
    std::vector<GaussianComponent> components(gaussianComponents.size());
    RandomInitializationRef initialization(spectrum.mzs, components, rngEngine);

    // Check if values of assigned means come from available mz values
    initialization.AssignRandomMeans();

    for (size_t i = 0; i < components.size(); i++)
    {
        EXPECT_NE(std::find(spectrum.mzs.begin(), spectrum.mzs.end(), components[i].mean), spectrum.mzs.end());
    }

    // Calculate sample variance
    initialization.AssignVariances();

    DataType sampleMean = std::accumulate(spectrum.mzs.begin(), spectrum.mzs.end(), 0.0) / (DataType)spectrum.mzs.size();
    DataType variance = 0.0;
    for (unsigned i = 0; i < spectrum.mzs.size(); i++)
    {
        variance += pow(spectrum.mzs[i] - sampleMean, 2);
    }
    variance /= (DataType)spectrum.mzs.size();
    DataType deviation = sqrt(variance);

    // Check if sample variance was properly assigned to all components
    for (unsigned i = 0; i < components.size(); i++)
    {
        EXPECT_EQ(components[i].deviation, deviation);
    }

    // Check if weights have been assigned uniformly
    initialization.AssignWeights();

    DataType weight = 1.0 / (DataType)components.size();
    for (unsigned i = 0; i < components.size(); i++)
    {
        EXPECT_EQ(components[i].weight, weight);
    }
}

TEST_F(ExpectationMaximizationTest, test_em_ref_expectation)
{
    Matrix<DataType> affilationMatrix((unsigned)gaussianComponents.size(), (unsigned)spectrum.mzs.size());

    ExpectationRunnerRef expectation(spectrum, affilationMatrix, gaussianComponents);
    expectation.Expectation();

    for (unsigned i = 0; i < spectrum.mzs.size(); i++)
    {
        DataType denominator = 0.0;
        for (unsigned k = 0; k < gaussianComponents.size(); k++)
        {
            denominator += gaussianComponents[k].weight *
                Gaussian(spectrum.mzs[i], gaussianComponents[k].mean, gaussianComponents[k].deviation);
        }

        for (unsigned k = 0; k < gaussianComponents.size(); k++)
        {
            DataType numerator = gaussianComponents[k].weight *
                Gaussian(spectrum.mzs[i], gaussianComponents[k].mean, gaussianComponents[k].deviation);
            EXPECT_EQ(affilationMatrix[k][i], numerator / denominator);
        }
    }
}


TEST_F(ExpectationMaximizationTest, test_em_ref_maximization)
{
    Matrix<DataType> affilationMatrix((unsigned)spectrum.mzs.size(), (unsigned)gaussianComponents.size());

    for (unsigned i = 0; i < spectrum.mzs.size(); i++)
    {
        for (unsigned k = 0; k < gaussianComponents.size(); k++)
        {
            affilationMatrix[i][k] = 0.1;
        }
    }

    MaximizationRunnerRef maximization(spectrum, affilationMatrix, gaussianComponents);

    // Check if means have ben properly updated
    maximization.UpdateMeans();

    for (unsigned k = 0; k < gaussianComponents.size(); k++)
    {
        DataType denominator = 0.0;
        DataType numerator = 0.0;
        for (unsigned i = 0; i < spectrum.mzs.size(); i++)
        {
            denominator += affilationMatrix[i][k] * spectrum.intensities[i];
            numerator += affilationMatrix[i][k] * spectrum.mzs[i] * spectrum.intensities[i];
        }
        EXPECT_EQ(gaussianComponents[k].mean, numerator / denominator);
    }

    // Check if standard deviations have been properly updated
    maximization.UpdateStdDeviations();

    for (unsigned k = 0; k < gaussianComponents.size(); k++)
    {
        DataType denominator = 0.0;
        DataType numerator = 0.0;
        for (unsigned i = 0; i < spectrum.mzs.size(); i++)
        {
            denominator += affilationMatrix[i][k] * spectrum.intensities[i];
            numerator += affilationMatrix[i][k] * pow(spectrum.mzs[i] - gaussianComponents[k].mean, 2) * spectrum.intensities[i];
        }
        EXPECT_EQ(gaussianComponents[k].deviation, sqrt(numerator / denominator));
    }

    // Check if standard deviations have been properly updated
    maximization.UpdateWeights();

    DataType totalDataSize = 0.0;
    for (unsigned i = 0; i < spectrum.mzs.size(); i++)
    {
        totalDataSize += spectrum.intensities[i];
    }

    const unsigned numberOfComponents = (unsigned)gaussianComponents.size();
    for (unsigned k = 0; k < numberOfComponents; k++)
    {
        DataType weight = 0.0;
        for (unsigned i = 0; i < spectrum.mzs.size(); i++)
        {
            weight += affilationMatrix[i][k] * spectrum.intensities[i];
        }
        EXPECT_EQ(gaussianComponents[k].weight, weight / totalDataSize);
    }
}

TEST_F(ExpectationMaximizationTest, test_em_opt_expectation)
{
    constexpr DataType MAX_ERROR = 0.001;
    Matrix<DataType> affilationMatrix((unsigned)gaussianComponents.size(),
        (unsigned)spectrum.mzs.size());
    ExpectationRunnerRef expectation(spectrum, affilationMatrix,
        gaussianComponents);
    expectation.Expectation();

    std::vector<Data> correctContents = {
        {
            4.13e-05, 8.4e-05, 0.000167, 0.000323, 0.000609, 0.00112, 0.00201,
            0.00353, 0.00602, 0.01, 0.0162, 0.0255, 0.039, 0.0578, 0.0829, 0.115,
            0.155, 0.2, 0.251, 0.304, 0.357, 0.408, 0.455, 0.496, 0.532, 0.561,
            0.583, 0.6, 0.61, 0.614, 0.612, 0.603, 0.588, 0.566, 0.537, 0.499,
            0.453, 0.399, 0.34, 0.278, 0.217, 0.162, 0.115, 0.0784, 0.0515, 0.0328,
            0.0204, 0.0124, 0.00745, 0.00441, 0.00258, 0.0015, 0.000862, 0.000492,
            0.000278, 0.000156, 8.66e-05, 4.75e-05, 2.57e-05, 1.37e-05, 7.12e-06,
            3.63e-06, 1.8e-06, 8.65e-07, 4.02e-07, 1.81e-07, 7.82e-08, 3.26e-08,
            1.31e-08, 5.11e-09, 1.93e-09, 7.05e-10, 2.5e-10, 8.65e-11, 2.91e-11,
            9.55e-12, 3.05e-12, 9.51e-13, 2.89e-13, 8.56e-14
        },
        {
            9.23e-15, 3.27e-14, 1.13e-13, 3.81e-13, 1.26e-12, 4.03e-12, 1.26e-11,
            3.85e-11, 1.14e-10, 3.32e-10, 9.36e-10, 2.57e-09, 6.84e-09, 1.77e-08,
            4.42e-08, 1.07e-07, 2.5e-07, 5.65e-07, 1.23e-06, 2.61e-06, 5.34e-06,
            1.06e-05, 2.07e-05, 3.93e-05, 7.33e-05, 0.000135, 0.000244, 0.000438,
            0.000776, 0.00136, 0.00237, 0.00407, 0.00691, 0.0116, 0.0191, 0.031,
            0.0491, 0.0754, 0.112, 0.159, 0.217, 0.282, 0.349, 0.415, 0.475,
            0.528, 0.572, 0.607, 0.634, 0.655, 0.668, 0.676, 0.677, 0.674, 0.664,
            0.649, 0.628, 0.6, 0.565, 0.524, 0.476, 0.423, 0.366, 0.306, 0.248,
            0.194, 0.147, 0.107, 0.0749, 0.0508, 0.0334, 0.0213, 0.0132, 0.00793,
            0.00465, 0.00266, 0.00148, 0.000804, 0.000426, 0.00022
        },
        {
            1, 1, 1, 1, 0.999, 0.999, 0.998, 0.996, 0.994, 0.99, 0.984, 0.975,
            0.961, 0.942, 0.917, 0.885, 0.845, 0.8, 0.749, 0.696, 0.643, 0.592,
            0.545, 0.504, 0.468, 0.439, 0.416, 0.4, 0.389, 0.385, 0.386, 0.392,
            0.405, 0.422, 0.444, 0.47, 0.498, 0.525, 0.548, 0.563, 0.566, 0.557,
            0.536, 0.507, 0.473, 0.439, 0.408, 0.38, 0.358, 0.341, 0.329, 0.323,
            0.322, 0.326, 0.335, 0.351, 0.372, 0.4, 0.434, 0.476, 0.524, 0.577,
            0.634, 0.694, 0.752, 0.806, 0.853, 0.893, 0.925, 0.949, 0.967, 0.979,
            0.987, 0.992, 0.995, 0.997, 0.999, 0.999, 1, 1
        }
    };
    Matrix<DataType> correctMatrix(correctContents);
    for (unsigned k = 0; k < gaussianComponents.size(); k++)
    {
        for (unsigned i = 0; i < spectrum.mzs.size(); i++)
        {
            EXPECT_NEAR(correctMatrix[k][i], affilationMatrix[k][i], MAX_ERROR);
        }
    }
}

TEST_F(ExpectationMaximizationTest, test_em_opt_maximization)
{
    constexpr DataType MAX_ERROR = 0.001;
    Matrix<DataType> affilationMatrix((unsigned)gaussianComponents.size(),
        (unsigned)spectrum.mzs.size());

    // Initialize affilationMatirx
    for (unsigned k = 0; k < gaussianComponents.size(); k++)
    {
        for (unsigned i = 0; i < spectrum.mzs.size(); i++)
        {
            affilationMatrix[k][i] = 0.1;
        }
    }

    MaximizationRunnerRef maximization(spectrum, affilationMatrix,
        gaussianComponents);

    // Check if means have ben properly updated
    maximization.UpdateMeans();
    maximization.UpdateStdDeviations();
    maximization.UpdateWeights();

    GaussianMixtureModel validComponents = {
    /*      mean       std      weight      */
        { -0.850374, 7.174315, 0.100000 },
        { -0.850374, 7.174315, 0.100000 },
        { -0.850374, 7.174315, 0.100000 }
    };
    ASSERT_EQ(validComponents.size(), gaussianComponents.size());
    for (Index i = 0; i < validComponents.size(); i++)
    {
        EXPECT_NEAR(gaussianComponents[i].mean, validComponents[i].mean, MAX_ERROR);
        EXPECT_NEAR(gaussianComponents[i].deviation, validComponents[i].deviation, MAX_ERROR);
        EXPECT_NEAR(gaussianComponents[i].weight, validComponents[i].weight, MAX_ERROR);
    }
}

TEST_F(ExpectationMaximizationTest, test_em_ref_loglikelihood)
{
    LogLikelihoodCalculator calculator(spectrum, gaussianComponents);

    DataType result = calculator.CalculateLikelihood();

    DataType sumOfLogs = 0.0;
    for (unsigned i = 0; i < spectrum.mzs.size(); i++)
    {
        DataType sum = 0.0;
        for (unsigned k = 0; k < gaussianComponents.size(); k++)
        {
            auto component = gaussianComponents[k];
            sum += component.weight * spectrum.intensities[i] * Gaussian(spectrum.mzs[i], component.mean, component.deviation);
        }
        sumOfLogs += log(sum);
    }

    ASSERT_EQ(result, sumOfLogs);
}
}
