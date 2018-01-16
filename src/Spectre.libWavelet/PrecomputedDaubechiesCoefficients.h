/*
 * PrecomputedDaubechiesCoefficients.h
 * Contains precomputed daubechie filters coefficients.
 *
   Copyright 2018 Michal Gallus

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
#include <array>
#include "DataTypes.h"

namespace spectre::algorithm::wavelet::precomputed
{
constexpr std::array<const DataType, 8> precomputedDaubechiesCoefficient{
    0.23037781330889653408355854935508,
    0.71484657055291567218091586255468,
    0.63088076792985892105036782595562,
    0.027983769416859854278767372193215,
    0.18703481171909294711319660109439,
    0.030841381835560857660460953866277,
    0.032883011666885168799989713761533,
    0.010597401785069055987764663484541
};
constexpr std::array<const DataType, 8> ReconstructionVlNumerator{
    precomputedDaubechiesCoefficient[0],
    precomputedDaubechiesCoefficient[1],
    precomputedDaubechiesCoefficient[2],
    -precomputedDaubechiesCoefficient[3],
    -precomputedDaubechiesCoefficient[4],
    precomputedDaubechiesCoefficient[5],
    precomputedDaubechiesCoefficient[6],
    -precomputedDaubechiesCoefficient[7]
};
constexpr std::array<const DataType, 8> DecompositionYhNumerator{
    -precomputedDaubechiesCoefficient[0],
    precomputedDaubechiesCoefficient[1],
    -precomputedDaubechiesCoefficient[2],
    -precomputedDaubechiesCoefficient[3],
    precomputedDaubechiesCoefficient[4],
    precomputedDaubechiesCoefficient[5],
    -precomputedDaubechiesCoefficient[6],
    -precomputedDaubechiesCoefficient[7]
};
constexpr std::array<const DataType, 8> DecompositionYlNumerator{ // @gmrukwa: reverse of f0
    -precomputedDaubechiesCoefficient[7],
    precomputedDaubechiesCoefficient[6],
    precomputedDaubechiesCoefficient[5],
    -precomputedDaubechiesCoefficient[4],
    -precomputedDaubechiesCoefficient[3],
    precomputedDaubechiesCoefficient[2],
    precomputedDaubechiesCoefficient[1],
    precomputedDaubechiesCoefficient[0]
};
constexpr std::array<const DataType, 8> ReconstructionVhNumerator{ // @gmrukwa: reverse of h1
    -precomputedDaubechiesCoefficient[7],
    -precomputedDaubechiesCoefficient[6],
    precomputedDaubechiesCoefficient[5],
    precomputedDaubechiesCoefficient[4],
    -precomputedDaubechiesCoefficient[3],
    -precomputedDaubechiesCoefficient[2],
    precomputedDaubechiesCoefficient[1],
    -precomputedDaubechiesCoefficient[0]
};
}
