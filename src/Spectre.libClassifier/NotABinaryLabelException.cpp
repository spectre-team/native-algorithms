/*
* NotABinaryLabelException
* Thrown when label was expected to be a binary
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

#include "NotABinaryLabelException.h"

namespace spectre::supervised::exception
{
NotABinaryLabelException::NotABinaryLabelException(Label label, size_t location, std::string collection):
    ExceptionBase("was: " + std::to_string(label) + ", at: " + std::to_string(location) + ", in: " + collection) { }
}