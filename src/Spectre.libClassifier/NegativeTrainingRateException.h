/*
* NegativeTrainingRateException.h
* Thrown when training rate is negative.
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
#include "Spectre.libException/ArgumentOutOfRangeException.h"

namespace spectre::supervised::exception
{
/// <summary>
/// Thrown, when training rate would be negative.
/// </summary>
class NegativeTrainingRateException final : public spectre::core::exception::ArgumentOutOfRangeException<double>
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="NegativeTrainingRateException"/> class.
    /// </summary>
    /// <param name="actual">The value of training rate.</param>
    explicit NegativeTrainingRateException(double actual);
};
}
