/*
* RaportGeneratorTest.cpp
* Tests RaportGenerator.
*
Copyright 2017 Grzegorz Mrukwa, Wojciech Wilgierz

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
#include "Spectre.GaSvmNative/RaportGenerator.h"
#include "Spectre.libException/ArgumentEqualZeroException.h"
#include "Spectre.libException/OpenFileException.h"
#include "Spectre.GaSvmNative/NegativeTrainingTimeException.h"
#include "Spectre.GaSvmNative/NegativeMeanClassificationTimeException.h"
#include "Spectre.GaSvmNative/InconsistentIndividualSizeAndNumberOfSupportVectorsException.h"

namespace
{
using namespace Spectre::GaSvmNative;
using namespace Spectre::libException;
using namespace Spectre::libClassifier;
using namespace Spectre::libGenetic;

class RaportGeneratorTest : public ::testing::Test
{
protected:
    std::vector<std::string> cells;
    char cell[40];
    const char* filename = "raportGeneratorTest";
    const char* badFilename = ".\test\raportGeneratorTest";
    const char* filenameCSV = "raportGeneratorTest.csv";
    const char* badFilenameCSV = ".\test\raportGeneratorTest.csv";
};

TEST_F(RaportGeneratorTest, throw_on_population_size_zero)
{
    std::remove(filenameCSV);
    EXPECT_THROW(RaportGenerator(filename, 0), ArgumentEqualZeroException);
}

TEST_F(RaportGeneratorTest, throw_on_failed_opened_file)
{
    std::remove(badFilenameCSV);
    EXPECT_THROW(RaportGenerator(badFilename, 2), OpenFileException);
}

TEST_F(RaportGeneratorTest, initializes_and_checks_created_file)
{
    std::remove(filenameCSV);
    EXPECT_NO_THROW(RaportGenerator(filename, 2));
    std::fstream fs;
    fs.open(filenameCSV, std::fstream::in);
    EXPECT_EQ(fs.is_open(), true);
    while (fs.good())
    {
        fs.getline(cell, 256, ',');
        cells.push_back(cell);
    }
    EXPECT_EQ(cells[0], "generation");
    EXPECT_EQ(cells[1], "true positives");
    EXPECT_EQ(cells[5], "Dice");
    EXPECT_EQ(cells[10], "number of support vectors");

    fs.close();
}

class RaportGeneratorInitializationTest : public ::testing::Test
{
public:
    RaportGeneratorInitializationTest():
        raportGenerator(writeFilename, populationSize),
        confusionMatrix(truePositives, trueNegatives, falsePositives, falseNegatives),
        individual({true, true}) {}

protected:
    const char* writeFilename = "raportGeneratorWriteTest";
    const unsigned int populationSize = 2;
    const double trainingTime = 10;
    const double classificationTime = 10;
    const double negativeTrainingTime = -10;
    const double negativeClassificationTime = -10;
    const unsigned int numberOfSupportVectors = 1;
    const unsigned int tooGreatNumberOfSupportVectors = 3;
    const unsigned int truePositives = 2;
    const unsigned int trueNegatives = 3;
    const unsigned int falsePositives = 4;
    const unsigned int falseNegatives = 5;
    const Spectre::libClassifier::ConfusionMatrix confusionMatrix;
    const Spectre::libGenetic::Individual individual;
    RaportGenerator raportGenerator;

    void setUp()
    {
    }
};

TEST_F(RaportGeneratorInitializationTest, write_function_no_throw)
{
    EXPECT_NO_THROW(raportGenerator.Write(confusionMatrix, individual, trainingTime, classificationTime, numberOfSupportVectors, nullptr));
}

TEST_F(RaportGeneratorInitializationTest, throw_on_negative_training_time)
{
    EXPECT_THROW(raportGenerator.Write(confusionMatrix, individual, negativeTrainingTime, classificationTime, numberOfSupportVectors, nullptr), NegativeTrainingTimeException);
}

TEST_F(RaportGeneratorInitializationTest, throw_on_negative_mean_classification_time)
{
    EXPECT_THROW(raportGenerator.Write(confusionMatrix, individual, trainingTime, negativeClassificationTime, numberOfSupportVectors, nullptr), NegativeMeanClassificationTimeException);
}

TEST_F(RaportGeneratorInitializationTest, throw_on_too_great_number_of_support_vectors)
{
    EXPECT_THROW(raportGenerator.Write(confusionMatrix, individual, trainingTime, classificationTime, tooGreatNumberOfSupportVectors, nullptr), InconsistentIndividualSizeAndNumberOfSupportVectorsException);
}

}
