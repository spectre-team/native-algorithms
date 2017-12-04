/*
* GaSvmScenario.h
* Handle to native scenario running GaSvm algorithm.
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
#include "Spectre.GaSvmNative/GeneticTrainingSetSelectionScenario.h"
#include "BridgedDataset.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Spectre::GaSvmCli;

namespace Spectre::GASVMCli
{
public ref class GaSvmScenario
{
public:
    GaSvmScenario(double trainingSetSplitRate,
                  double mutationRate,
                  double bitSwapRate,
                  double preservationRate,
                  unsigned int generationsNumber,
                  array<unsigned int>^ populationSizes,
                  array<unsigned int>^ initialFillups,
                  String^ reportFilename,
                  unsigned int numberOfRestarts,
                  unsigned int numberOfCores,
                  unsigned long long seed,
                  unsigned long long minimalFillup,
                  unsigned long long maximalFillup,
                  unsigned int svmIterations,
                  double svmTolerance):
        m_GaSvmNative(new GaSvmNative::GeneticTrainingSetSelectionScenario(trainingSetSplitRate,
                                                                          mutationRate,
                                                                          bitSwapRate,
                                                                          preservationRate,
                                                                          generationsNumber,
                                                                          toNative(populationSizes),
                                                                          toNative(initialFillups),
                                                                          toNative(reportFilename),
                                                                          numberOfRestarts,
                                                                          numberOfCores,
                                                                          seed,
                                                                          minimalFillup,
                                                                          maximalFillup,
                                                                          svmIterations,
                                                                          svmTolerance))
    {
        
    }

    void execute(BasicTextDataset^ data, BasicTextDataset^ independentValidation)
    {
        BridgedDataset^ dataBridge = gcnew BridgedDataset(data);
        const auto nativeData = dataBridge->GetNative();
        BridgedDataset^ validationBridge;
        libClassifier::OpenCvDataset* independentNative = nullptr;
        if(independentValidation != nullptr)
        {
            validationBridge = gcnew BridgedDataset(independentValidation);
            independentNative = validationBridge->GetNative();
        }
        
        m_GaSvmNative->execute(*nativeData, independentNative);
    }

    !GaSvmScenario()
    {
        delete m_GaSvmNative;
    }

    ~GaSvmScenario()
    {
        this->!GaSvmScenario();
    }

private:
    const GaSvmNative::GeneticTrainingSetSelectionScenario* m_GaSvmNative;

    template <typename T>
    static std::vector<T> toNative(array<T>^ managedCollection)
    {
        std::vector<T> native;
        native.reserve(managedCollection->Length);
        for (auto i=0; i < managedCollection->Length; ++i)
        {
            native.push_back(managedCollection[i]);
        }
        return native;
    }

    std::string toNative(String^ managedString)
    {
        size_t length = managedString->Length;
        std::string native(length, '\0');
        for(auto i=0u; i<length; ++i)
        {
            // @gmrukwa: TODO: Migrate to something more up-to-date.
            native[i] = static_cast<char>(managedString[i]);
        }
        return native;
    }
};
}