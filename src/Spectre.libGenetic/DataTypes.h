/*
* DataTypes.h
* Types & aliases used in this library.
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

#pragma once
#include <random>
#include "Generation.h"

namespace spectre::algorithm::genetic
{
using ScoreType = double;
using RandomDevice = std::random_device;
using RandomNumberGenerator = std::mt19937_64;
using Seed = _ULonglong; // @gmrukwa: from mt19937_64
}
