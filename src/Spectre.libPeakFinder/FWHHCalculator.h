/*
* FWHHCalculator.h
* Class for calculating locations of full width at half height markers for 
* input peak data.
*
Copyright 2018 Dariusz Kuchta

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
#include "DataTypes.h"

namespace spectre::algorithm::peakfinder
{
    class FWHHCalculator
    {
    public:
        Signal GetLeftFWHH(const SignalView x, const SignalView y, const IndicesView valleys, const IndicesView peaks);
        Signal GetRightFWHH(const SignalView x, const SignalView y, const IndicesView valleys, const IndicesView peaks);
    private:
        inline Index GetClosestNeighbourIndex(const SignalView& sorted_x, const DataType value);
        inline DataType Lerp(DataType x1, DataType x2, DataType y1, DataType y2, DataType x);
    };
}
