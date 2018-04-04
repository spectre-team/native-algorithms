/*
* ClassifierFactory.cpp
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

#include "ClassifierFactory.h"
#include "Spectre.libClassifier/Svm.h"

namespace spectre::supervised
{

ClassifierFactory::ClassifierFactory() {}

Svm ClassifierFactory::buildSvm(unsigned iterationsLimit, double tolerance) const
{
    Svm svm(iterationsLimit, tolerance);
    return svm;
}

GaClassifier ClassifierFactory::buildGaClassifier(
    std::string name,
    double trainingSetSplitRate,
    double mutationRate,
    double bitSwapRate,
    double preservationRate,
    unsigned generationsNumber,
    unsigned populationSize,
    unsigned initialFillup,
    spectre::algorithm::genetic::Seed seed,
    size_t minimalFillup,
    size_t maximalFillup,
    unsigned int iterationsLimit,
    double tolerance) const
{
    std::unique_ptr<IClassifier> classifier;
    if (name == "svm")
    {
        Svm svm = this->buildSvm(iterationsLimit, tolerance);
        classifier = std::make_unique<Svm>(svm);
    }
    GaClassifier gaClassifier(std::move(classifier), trainingSetSplitRate, mutationRate, bitSwapRate, preservationRate, generationsNumber,
        populationSize, initialFillup, seed, minimalFillup, maximalFillup);
    return gaClassifier;
}


}
