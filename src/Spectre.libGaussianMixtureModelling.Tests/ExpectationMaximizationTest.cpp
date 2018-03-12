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
#include "ExpectationMaximization.h"
#include "RandomInitializationRef.h"
#include "ExpectationRunnerRef.h"
#include "MaximizationRunnerRef.h"
#include "LogLikelihoodCalculator.h"

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

    ExpectationMaximization<
        RandomInitializationRef,
        ExpectationRunnerRef,
        MaximizationRunnerRef,
        LogLikelihoodCalculator
    > em(SpectrumView(spectrum),
         rngEngine, (unsigned)gaussianComponents.size());

    GaussianMixtureModel result = em.EstimateGmm();
    Data approximates = GenerateValues(spectrum.mzs, result.components);

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
    Matrix affilationMatrix((unsigned)gaussianComponents.size(), (unsigned)spectrum.mzs.size());

    ExpectationRunnerRef expectation(spectrum.mzs, affilationMatrix, gaussianComponents);
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
            EXPECT_EQ(affilationMatrix[i][k], numerator / denominator);
        }
    }
}

TEST_F(ExpectationMaximizationTest, test_em_ref_maximization)
{
    Matrix affilationMatrix((unsigned)gaussianComponents.size(), (unsigned)spectrum.mzs.size());

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
