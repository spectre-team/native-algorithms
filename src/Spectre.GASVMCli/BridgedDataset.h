/*
* BridgedDataset.h
* Bridge class for C# dataset representation and native OpenCV implementation.
*
Copyright 2017 Dariusz Kuchta

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

#include <gsl.h>
#include "OpenCvDataset.h"

using namespace System;
using namespace Spectre::Data::Datasets;
using namespace Spectre::libClassifier;

namespace Spectre::SpectreGASVMCli {

	public ref class BridgedDataset
	{
    public:
	    BridgedDataset(BasicTextDataset^ managedDataset)
		{
            auto datasetLength = managedDataset->SpectrumCount *
                managedDataset->SpectrumLength;
            std::vector<float> data(datasetLength);
            for(int i = 0; i < managedDataset->SpectrumCount; ++i)
            {
                auto intensities = managedDataset->GetRawIntensityArray(i);
                for(int j = 0; j < managedDataset->SpectrumLength; ++j)
                {
                    auto index = i * managedDataset->SpectrumLength + j;
                    data[index] = static_cast<float>(intensities[j]);
                }
            }
            //auto labels = managedDataset.Labels;
            m_openCvDataset = new OpenCvDataset(data, nullptr);
		}

        ~BridgedDataset()
	    {
            delete m_openCvDataset;
	    }

        OpenCvDataset *GetNative()
	    {
            return m_openCvDataset;
	    }

    private:
        OpenCvDataset *m_openCvDataset;
	};
}
