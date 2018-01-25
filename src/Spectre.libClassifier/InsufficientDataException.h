/*
* InsufficientDataException.h
* Throws when data does not cover all classes.
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
#include "Spectre.libException/ExceptionBase.h"
#include "NotABinaryLabelException.h"

namespace spectre::supervised::exception
{
    /// <summary>
    /// Throws when data does not cover all classes.
    /// </summary>
    class InsufficientDataException final : public core::exception::ExceptionBase
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="InsufficientDataException"/> class.
        /// </summary>
        /// <param name="actual">The missing class .</param>
        explicit InsufficientDataException(Label missingClass);
    };
}
