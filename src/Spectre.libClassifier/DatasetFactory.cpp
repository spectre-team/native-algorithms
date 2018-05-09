/*
* DatasetFactory.cpp
* Creates OpenCvDataset from files.
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
DatasetFactory::DatasetFactory()
{
}

//There is no file format validity for now, it will just throw an error during
//OpenCvDataset creation from wrong read data.
//Needs to be fixed in the future. 
OpenCvDataset DatasetFactory::create(const std::string& filename)
{
    std::fstream file;
    file.open(filename, std::fstream::in);
    if (!file.is_open())
    {
        throw exception::FileOpenException(filename);
    }
    std::vector<DataType> data = {};
    std::vector<Label> labels = {};
    DataType dataType;
    std::string line;
    {
        //we need to skip metadata line, however I don't know exactly why,
        //but for it to work, we need 2 getlines
        getline(file, line);
        getline(file, line);
    }
    while (!file.eof())
    {
        //get Label
        getline(file, line);
        if (!line.empty()) {
            //in text files in lines with coordinates and label values, we skip coordinates 
            //and only get the last number (label)
            labels.push_back(readLabel(line));
            
            //we push every number in lines in text file containing data to "data" variable
            getline(file, line);
            std::istringstream ssData(line);
            while (ssData >> dataType)
            {
                data.push_back(dataType);
            }
        }
    }
    return OpenCvDataset(data, labels);
}

Label DatasetFactory::readLabel(const std::string& line)
{
    Label label;
    std::istringstream ssLabels(line);
    while (ssLabels >> label) {}
    return label;
}
}
