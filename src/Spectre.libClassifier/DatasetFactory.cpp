/*
* DatasetFactory.cpp
* Creates OpenCvDataset from external sources.
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

#include <fstream>
#include <sstream>
#include "DatasetFactory.h"
#include "FileOpenException.h"

namespace spectre::supervised
{
const int ColumnMatrixWidth = 1;

DatasetFactory::DatasetFactory()
{
}

OpenCvDataset DatasetFactory::create(std::string filename) const
{
    std::fstream file;
    file.open(filename, std::fstream::in);
    if (!file.is_open())
    {
        throw exception::FileOpenException(filename);
    }
    std::vector<DataType> m_Data = {};
    std::vector<Label> m_labels = {};
    DataType num;
    Label label;
    std::string line;
    {
        getline(file, line);
        getline(file, line);
    }
    while (!file.eof())
    {
        //get Label
        getline(file, line);
        if (line != "") {
            std::istringstream ssLabels(line);
            while (ssLabels >> label) {}
            m_labels.push_back(label);

            //get DataTypes
            getline(file, line);
            std::istringstream ssData(line);
            while (ssData >> num)
            {
                m_Data.push_back(num);
            }
        }
    }
    OpenCvDataset openCvDataset(m_Data, m_labels);
    return openCvDataset;
}
}
