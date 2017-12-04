/*
* RandomMultipleValidationSelection.h
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
#include "Spectre.GaSvmNative\RandomMultipleValidationSelectionScenario.h"
#include "BridgedDataset.h"
#include "Utilities.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Spectre::GaSvmCli;

namespace Spectre::GASVMCli
{
    public ref class RandomMultipleValidationSelection
    {
    public:
        RandomMultipleValidationSelection(double trainingSetSplitRate,
            double downsampleTrainingRate,
            unsigned long long maximumSubsetSize,
            double mutationRate,
            double bitSwapRate,
            double preservationRate,
            unsigned int numberOfGenerations,
            unsigned int populationSize,
            unsigned int initialFillup,
            String^ filename,
            unsigned int numberOfRestarts,
            unsigned int numberOfCores,
            unsigned long long seed,
            unsigned long long minimalFillup,
            unsigned long long maximalFillup,
            unsigned int svmIterations,
            double svmTolerance): 
        m_GaSvmNative(new GaSvmNative::RandomMultipleValidationSelectionScenario(trainingSetSplitRate,
                                                                                downsampleTrainingRate,
                                                                                maximumSubsetSize,
                                                                                mutationRate,
                                                                                bitSwapRate,
                                                                                preservationRate,
                                                                                numberOfGenerations,
                                                                                populationSize,
                                                                                initialFillup,
                                                                                toNative(filename),
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
            if (independentValidation != nullptr)
            {
                validationBridge = gcnew BridgedDataset(independentValidation);
                independentNative = validationBridge->GetNative();
            }

            m_GaSvmNative->execute(*nativeData, independentNative);
        }

        !RandomMultipleValidationSelection()
        {
            delete m_GaSvmNative;
        }

        ~RandomMultipleValidationSelection()
        {
            this->!RandomMultipleValidationSelection();
        }
        
    private:
        const GaSvmNative::RandomMultipleValidationSelectionScenario* m_GaSvmNative;
    };

}