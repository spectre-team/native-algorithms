/*
* DatasetFactoryTest.cpp
* Tests DatasetFactory class.
*
Copyright 2017 Wojciech Wilgierz

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
#include <fstream>
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libClassifier/DatasetFactory.h"
#include "Spectre.libClassifier/FileOpenException.h"

namespace
{
using namespace spectre::supervised;

TEST(DatasetFactoryInitializationTest, creates_opencv_dataset_from_file)
{
    EXPECT_NO_THROW(DatasetFactory());
}

class DatasetFactoryTest : public ::testing::Test
{
public:
    DatasetFactoryTest() :
        datasetFactory() {}
protected:
    const std::vector<DataType> dataCompare{ 1.1f, 4.5f, 6.7f, 5.0f, 9.5f, 3.9f, 7.1f, 5, 3.2f, 10, 24.5f, 5.756f,
        5.09f, 9.6f, 8.23452f, 2, 5.53f, 3.76f, 6.11234f, 6.9f, 7.0f, };
    const std::vector<Label> labelsCompare{ 0, 0, 1, 1, 0, 0, 1 };
    const std::string filename = "test_file.txt";
    const std::string nonExistantFilename = "non_existant_file.txt";
    const std::vector<std::string> dataForFile = { "test file", "2.6 9.5 1.0", "0 0 0 0", "1.1 4.5 6.7", "1 0 0 0",
        "5.0 9.5 3.9", "0 1 0 1", "7.1 5 3.2", "1 1 0 1", "10 24.5 5.756", "1 2 0 0", "5.09 9.6 8.23452", "2 1 0 0",
        "2 5.53 3.76", "2 2 0 1", "6.11234 6.9 7.0" };
    std::fstream file;
    DatasetFactory datasetFactory;
};

TEST_F(DatasetFactoryTest, throws_on_non_existant_file)
{
    EXPECT_THROW(OpenCvDataset datasetFromFile = datasetFactory.create(nonExistantFilename), exception::FileOpenException);
}

TEST_F(DatasetFactoryTest, compare_dataset_from_file_and_from_data)
{
    file.open(filename, std::fstream::out);
    for (std::string str: dataForFile)
    {
        file << str << std::endl;
    }
    file.close();

    OpenCvDataset datasetCompare(dataCompare, labelsCompare);
    OpenCvDataset datasetFromFile = datasetFactory.create(filename);
    for (auto i = 0u; i < datasetCompare.size(); i++)
    {
        for (auto j = 0; j < datasetCompare[i].size(); j++)
        {
            EXPECT_FLOAT_EQ(datasetCompare[i][j], datasetFromFile[i][j]);
        }
        EXPECT_EQ(datasetCompare.GetSampleMetadata(i), datasetFromFile.GetSampleMetadata(i));
    }
}
}
