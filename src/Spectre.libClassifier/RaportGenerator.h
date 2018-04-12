/*
* RaportGenerator.h
* Class that has static functions to create raport during for example genetic algorithm execution.
*
Copyright 2017 Spectre Team

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

#pragma once
#include <fstream>
#include <omp.h>
#include "ConfusionMatrix.h"

namespace spectre::supervised
{
class RaportGenerator final
{
public:
    explicit RaportGenerator(std::string filename,
        unsigned int populationSize,
        const std::string& separator = ",");
    void Write(const ConfusionMatrix& matrix,
        const std::vector<bool> individual,
        unsigned int numberOfSupportVectors);
    ~RaportGenerator();
private:
    std::ofstream m_File;
    const std::string m_Separator;
    unsigned int m_IndividualsProcessed;
    const unsigned int m_PopulationSize;
    const std::string m_Filenane;
    omp_lock_t m_WriteLock;
};
}
