/*
* ClassifierFactory.h
* The factory for creating classifiers
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

#pragma once
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libGenetic/DataTypes.h"
#include "GaClassifier.h"

namespace spectre::supervised
{
/// <summary>
/// The factory for creating classifiers
/// </summary>
class ClassifierFactory
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ClassifierFactory"/> class.
    /// </summary>
    ClassifierFactory();
    /// <summary>
    /// Gets svm classifier
    /// </summary>
    Svm buildSvm(unsigned int iterationsLimit = 100, double tolerance = 1e-6) const;
    /// <summary>
    /// Gets GaClassifier
    /// </summary>
    GaClassifier buildGaClassifier(RaportGenerator& raport,
        std::string name,
        double trainingSetSplitRate,
        double mutationRate,
        double bitSwapRate,
        double preservationRate,
        unsigned int generationsNumber,
        unsigned int populationSize,
        unsigned int initialFillup,
        spectre::algorithm::genetic::Seed seed = 0,
        size_t minimalFillup = 1ul,
        size_t maximalFillup = std::numeric_limits<size_t>::max(),
        unsigned int iterationsLimit = 100,
        double tolerance = 1e-6) const;
};
}
