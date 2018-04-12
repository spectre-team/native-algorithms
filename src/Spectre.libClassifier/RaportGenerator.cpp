/*
* RaportGenerator.cpp
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

#include "Spectre.libClassifier/ConfusionMatrix.h"
#include "RaportGenerator.h"

namespace spectre::supervised
{
RaportGenerator::RaportGenerator(std::string filename, uint populationSize, const std::string& separator) :
    m_Separator(separator),
    m_IndividualsProcessed(0),
    m_PopulationSize(populationSize),
    m_Filenane(filename)
{
    m_File.open(filename + ".csv");
    m_File << "generation" << m_Separator;
    m_File << "true positives" << m_Separator;
    m_File << "true negatives" << m_Separator;
    m_File << "false positives" << m_Separator;
    m_File << "false negatives" << m_Separator;
    m_File << "Dice" << m_Separator;
    m_File << "number of observations used" << m_Separator;
    m_File << "percent of observations used" << m_Separator;
    m_File << "number of support vectors" << m_Separator;
    m_File.flush();
    omp_init_lock(&m_WriteLock);
}

void RaportGenerator::Write(const ConfusionMatrix& matrix,
    const std::vector<bool> individual,
    unsigned int numberOfSupportVectors)
{
    auto count = 0u;
    for (auto bit : individual)
    {
        count += bit;
    }

    omp_set_lock(&m_WriteLock);
    m_File << m_IndividualsProcessed++ / m_PopulationSize << m_Separator;
    m_File << matrix.TruePositive << m_Separator;
    m_File << matrix.TrueNegative << m_Separator;
    m_File << matrix.FalsePositive << m_Separator;
    m_File << matrix.FalseNegative << m_Separator;
    m_File << matrix.DiceIndex << m_Separator;
    m_File << count << m_Separator;
    m_File << static_cast<double>(count) / individual.size() << m_Separator;
    m_File << numberOfSupportVectors << m_Separator;
    m_File << "\n";
    m_File.flush();
    omp_unset_lock(&m_WriteLock);
}


RaportGenerator::~RaportGenerator()
{
    m_File.close();
    omp_destroy_lock(&m_WriteLock);
}

}
