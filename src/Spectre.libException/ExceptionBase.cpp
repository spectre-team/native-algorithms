/*
* ExceptionBase.cpp
* Common exception base for all native libraries.
*
Copyright 2017 Grzegorz Mrukwa

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

#include "ExceptionBase.h"

namespace spectre::core::exception
{
ExceptionBase::ExceptionBase(const std::string message): m_message(message) { }

char const* ExceptionBase::what() const noexcept
{
    return m_message.c_str();
}
}
