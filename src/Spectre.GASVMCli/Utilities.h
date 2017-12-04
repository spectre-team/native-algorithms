/*
* Utilities.h
* Helper functions providing useful functionalities for the CLI project.
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

using namespace System;
using namespace System::Collections::Generic;

namespace Spectre::GASVMCli
{
    template <typename T>
    std::vector<T> toNative(array<T>^ managedCollection)
    {
        std::vector<T> native;
        native.reserve(managedCollection->Length);
        for (auto i = 0; i < managedCollection->Length; ++i)
        {
            native.push_back(managedCollection[i]);
        }
        return native;
    }

    std::string toNative(String^ managedString)
    {
        size_t length = managedString->Length;
        std::string native(length, '\0');
        for (auto i = 0u; i<length; ++i)
        {
            // @gmrukwa: TODO: Migrate to something more up-to-date.
            native[i] = static_cast<char>(managedString[i]);
        }
        return native;
    }
}