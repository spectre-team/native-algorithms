/*
* DatasetFactory.cpp
* Creates OpenCvDataset from external sources like file.
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
#include "Spectre.libException/InconsistentArgumentSizesException.h"

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
    std::vector<DataType> m_Data = {};
    std::vector<Label> m_labels = {};
    {
        std::string tmp;
        getline(file, tmp);
    }
    while (!file.eof())
    {
        std::string data;
        getline(file, data);
        std::istringstream ssData(data);
        DataType num;
        while (ssData >> num)
        {
            m_Data.push_back(num);
        }
        std::string labels;
        getline(file, labels);
        std::istringstream ssLabels(labels);
        DataType label;
        while (ssLabels >> label) {}
        m_labels.push_back(static_cast<Label>(label));
    }
    OpenCvDataset openCvDataset(m_Data, m_labels);
    return openCvDataset;
}
}
