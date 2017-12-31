﻿/*
* EmptyOpenCvDatasetException.h
* Exception thrown, when we would create empty OpenCvDataset.
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
#include "Spectre.libException/ExceptionBase.h"

namespace spectre::supervised::exception
{
    /// <summary>
    /// Thrown, when we would create empty OpenCvDataset.
    /// </summary>

    class EmptyOpenCvDatasetException final : public core::exception::ExceptionBase
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="EmptyOpenCvDatasetException"/> class.
        /// </summary>
        /// <param name="">Name of null variable.</param>
        explicit EmptyOpenCvDatasetException(const std::string &variableName);
    };
}
