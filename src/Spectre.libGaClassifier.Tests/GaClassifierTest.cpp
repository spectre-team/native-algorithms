/*
* GaClassifierTest.cpp
* Tests GaClassifier class.
*
Copyright 2018 Spectre Team

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

#include <gtest/gtest.h>
#include "Spectre.libClassifier/Types.h"
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libGaClassifier/GaClassifier.h"

namespace spectre::supervised
{
const double TRAINING_SET_SPLIT_RATE = 0.7;
const double MUTATION_RATE = 0.5;
const double BIT_SWAP_RATE = 0.5;
const double PRESERVATION_RATE = 0.5;
const unsigned int GENERATIONS_NUMBER = 5u;
const unsigned int POPULATION_SIZE = 5u;
const unsigned int INITIAL_FILLUP = 3u;
const Seed SEED = 1;

TEST(GaClassifierInitialization, initializes)
{
    const std::vector<DataType> data{ 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f,
        0.4f, 1.6f, 0.9f, 1.2f, 2.2f, 0.7f, 1.3f, 2.0f, 1.4f, 0.7f, 0.7f, 0.9f };
    const std::vector<Label> labels{ 1, 1, 0, 1, 0, 0, 1 };
    OpenCvDataset dataset(std::move(data), std::move(labels));
    Svm svm{};

    EXPECT_NO_THROW(GaClassifier gaClassifier(svm,
        TRAINING_SET_SPLIT_RATE,
        MUTATION_RATE,
        BIT_SWAP_RATE,
        PRESERVATION_RATE,
        GENERATIONS_NUMBER,
        POPULATION_SIZE,
        INITIAL_FILLUP));
}

class GaClassifierTest : public ::testing::Test
{
public:
    GaClassifierTest():
        dataset(std::move(data), std::move(labels)),
        gaClassifier(svm,
            TRAINING_SET_SPLIT_RATE,
            MUTATION_RATE,
            BIT_SWAP_RATE,
            PRESERVATION_RATE,
            GENERATIONS_NUMBER,
            POPULATION_SIZE,
            INITIAL_FILLUP,
            SEED) {}

protected:
    const std::vector<DataType> data{ 0.5f, 0.4f, 0.6f, 1.1f, 1.6f, 0.7f, 2.1f, 1.0f, 0.6f, 0.4f, 0.6f, 1.1f,
        0.4f, 1.6f, 0.9f, 1.2f, 2.2f, 0.7f, 1.3f, 2.0f, 1.4f, 0.7f, 0.7f, 0.9f, 1.6f, 0.7f, 2.1f, 0.7f, 1.3f, 2.0f };
    const std::vector<Label> labels{ 1, 1, 0, 1, 0, 0, 1, 0, 1, 0 };
    OpenCvDataset dataset;
    Svm svm{};
    GaClassifier gaClassifier;
};

TEST_F(GaClassifierTest, fits_with_no_exception)
{
    EXPECT_NO_THROW(gaClassifier.Fit(dataset));
}

TEST_F(GaClassifierTest, predicts_with_no_throw)
{
    gaClassifier.Fit(dataset);
    EXPECT_NO_THROW(gaClassifier.Predict(dataset));
}

}
