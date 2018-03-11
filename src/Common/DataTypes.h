/*
 * DataTypes.h
 * Contains data types used among the solution.
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
#include <vector>
#include <span.h>

namespace spectre
{
using DataType = double;
using Index = unsigned;

using Data = std::vector<DataType>;
using Indices = std::vector<Index>;

using DataView = gsl::span<DataType>;
using IndicesView = gsl::span<Index>;
}