/*
* InconsistentMinimalAndMaximalFillupException.cpp
* Thrown when minimal fillup is greater than maximal.
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

#include <string>
#include "InconsistentMinimalAndMaximalFillupException.h"

namespace spectre::algorithm::genetic
{
InconsistentMinimalAndMaximalFillupException::InconsistentMinimalAndMaximalFillupException(size_t minimal, size_t maximal) :
    ExceptionBase("minimal value of fillup: " + std::to_string(minimal) + " is greater than its maximum value: " + std::to_string(maximal)) { }

InconsistentMinimalAndMaximalFillupException::InconsistentMinimalAndMaximalFillupException(double minimal, double maximal) :
    ExceptionBase("minimal value of fillup: " + std::to_string(minimal) + " is greater than its maximum value: " + std::to_string(maximal)) { }

}
