/*
* Individual.cpp
* Binary individual class.
*
Copyright 2017 Grzegorz Mrukwa, Wojciech Wilgierz

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

#include <vector>
#include <algorithm>
#include "Spectre.libException/OutOfRangeException.h"
#include "Individual.h"
#include "InconsistentChromosomeLengthException.h"

using namespace std;
using namespace spectre::core::exception;

namespace spectre::algorithm::genetic
{
Individual::Individual(std::vector<bool> &&binaryData):
    std::vector<bool>(binaryData) { }

bool Individual::operator==(const Individual &other) const
{
    return std::equal(begin(), end(), other.begin(), other.end());
}

bool Individual::operator!=(const Individual &other) const
{
    return !(*this == other);
}

Individual& Individual::operator=(const Individual &other)
{
    if (size() == other.size())
    {
        assign(other.begin(), other.end());
        return *this;
    }
    else
    {
        throw InconsistentChromosomeLengthException(size(), other.size());
    }
}

}
