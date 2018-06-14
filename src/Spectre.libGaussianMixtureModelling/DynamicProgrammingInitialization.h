/*
* DynamicProgrammingInitialization.h
* Contains functions that implement the dynamic initialization.
* For algorithm description, please refer to:
* https://www.worldscientific.com/doi/pdf/10.1142/S0219876218500123
* All equation references correspond to the paper above.
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
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include <limits>
#include "Spectre.libGaussianMixtureModelling/Matrix.h"
#include <numeric>
#include "Spectre.libException/OutOfRangeException.h"


namespace spectre::unsupervised::gmm
{
using namespace spectre::core::exception;
static inline Data InitRightmostBlockQualities(SpectrumView, DataType, DataType);
static inline void ComputeOptimalBlockRanges(Data&, Data&, Matrix<Index>&,
    Matrix<DataType>&, unsigned);
static inline void ComputePartitionSeparators(Indices&, Matrix<Index>&, unsigned,
    unsigned);

/// <summary>
/// Computes a basic quality of a block based on given block data. Equation (28)
/// </summary>
/// <param name="blockData">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero.</param>
/// <param name="intensitySum">Sum of intensity values in blockData.</param>
/// <returns>Quality Q1 of the block</returns>
DataType ComputeBasicQuality(SpectrumView blockData, DataType intensitySum)
{
    const DataView& mzs = blockData.mzs;
    const DataView& intensities = blockData.intensities;
    DataType sumInverse = 1. / intensitySum; // Since * is cheaper than /
    std::vector<DataType> normalizedHeights(mzs.length()); // (16) in the paper
    DataType innerSum = 0;
    for (unsigned i = 0; i < (unsigned)mzs.length(); i++)
    {
        DataType normalizedHeight = intensities[i] * sumInverse;
        normalizedHeights[i] = normalizedHeight;
        innerSum += mzs[i] * normalizedHeight;
    }
    DataType quality = 0.0;
    for (unsigned i = 0; i < (unsigned)mzs.length(); i++)
    {
        quality += pow(mzs[i] - innerSum, 2) * normalizedHeights[i]; // (28) in the paper
    }

    return quality;
}

/// <summary>
/// Computes a quality of a block based on given block data. Equation (27)
/// </summary>
/// <param name="blockData">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero. </param>
/// <param name="delta">A positive constant from equation (27).</param>
/// <param name="minStd">Smallest mz range for which calculated
/// range quality attains a positive finite number.</param>
/// <returns>Quality Q4 of the block.</returns>
DataType ComputeBlockQuality(SpectrumView blockData, DataType delta, DataType minStd)
{
    constexpr DataType MIN_SUMMED_HEIGHT = 0.001;
    const DataView mzs = blockData.mzs;
    const DataView intensities = blockData.intensities;

    DataType mzRange = *(mzs.end() - 1) - *(mzs.begin());
    bool failsWidthRequirement = mzRange <= minStd;
    DataType sum = std::accumulate(intensities.begin(), intensities.end(), 0.0);
    bool failsSummedHeightRequirement = sum <= MIN_SUMMED_HEIGHT;

    if (failsSummedHeightRequirement || failsWidthRequirement)
    {
        return std::numeric_limits<DataType>::infinity();
    }

    const DataType indexOneQuality = ComputeBasicQuality(blockData, sum);
    return (delta + sqrt(indexOneQuality)) / mzRange;
}

/// <summary>
/// Computes what are optimal index ranges for given number of blocks
/// in provided signal.
/// </summary>
/// <param name="spectrum">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero. </param>
/// <param name="numOfBlocks">Number of index ranges to compute.</param>
/// <param name="qualityOfRange">Precomputed qualities of all possible mz
/// ranges.</param>
/// <param name="delta">A positive constant from equation (27).</param>
/// <param name="minStd">Smallest mz range for which calculated
/// range quality attains a positive finite number.</param>
/// <exception cref="OutOfRangeException">Thrown when number of blocks is
/// greater than signal length - 2.</exception>
/// <exception cref="std::invalid_argument">Thrown when either delta or minStd
/// are non-positive numbers.</exception>
/// <returns>List of block ranges. First index symbolises the beginning of the
/// first block, while the last - the end of the last block. Every other index
/// symbolises the end of the block, and the beginning of the next one.</returns>
Indices ComputeOptimalBlocks(SpectrumView spectrum, unsigned numOfBlocks,
    Matrix<DataType>& qualityOfRange, DataType delta, DataType minStd)
{
    if (numOfBlocks > unsigned(spectrum.mzs.size() - 2))
        throw OutOfRangeException(numOfBlocks, spectrum.mzs.size() - 2);
    if (delta < 0.0 || minStd < 0.0)
        throw std::invalid_argument("Delta and minStd need to be positive");
    const unsigned mzCount = (unsigned) spectrum.mzs.size();
    Data qualitySums(mzCount);
    Matrix<Index> indicesOfOptimalBlockRanges(numOfBlocks, mzCount);
    Data currentOptimalQuality =
        std::move(InitRightmostBlockQualities(spectrum, delta, minStd));
    // currentOptQualityIndex contains Q^opt(1) at this point.
    ComputeOptimalBlockRanges(currentOptimalQuality, qualitySums,
        indicesOfOptimalBlockRanges, qualityOfRange, numOfBlocks);

    Indices partitionSeparators(numOfBlocks + 2); // + 2 for the last and first index.
    ComputePartitionSeparators(partitionSeparators, indicesOfOptimalBlockRanges,
        numOfBlocks, mzCount);
    return partitionSeparators;
}

static inline Data InitRightmostBlockQualities(SpectrumView spectrum, DataType delta,
    DataType minStd)
{
    const unsigned mzCount = (unsigned)spectrum.mzs.size();
    Data qualities(mzCount);
    for (unsigned mzIndex = 0; mzIndex < mzCount; mzIndex++)
    {
        unsigned subSpanSize = mzCount - mzIndex;
        qualities[mzIndex] =
            ComputeBlockQuality(spectrum.subspan(mzIndex, subSpanSize), delta, minStd);
    }
    return qualities;
}

static inline void ComputeOptimalBlockRanges(Data& currentOptimalQuality,
    Data& qualitySums, Matrix<Index>& indicesOfOptimalBlockRanges,
    Matrix<DataType>& qualityOfRange, unsigned numOfBlocks)
{
    const unsigned mzCount = (unsigned)qualitySums.size();
    for (Index blockIndex = 0; blockIndex < numOfBlocks; blockIndex++)
    {
        const unsigned anchorLimit = mzCount - blockIndex;
        for (Index mzAnchor = 0; mzAnchor < anchorLimit - 1; mzAnchor++)
        {
            DataType minQuality = std::numeric_limits<DataType>::infinity();
            Index minPos = mzAnchor + 1;
            for (Index mzLimit = minPos; mzLimit < anchorLimit; mzLimit++)
            {
                qualitySums[mzLimit] =
                    qualityOfRange[mzAnchor][mzLimit] + currentOptimalQuality[mzLimit];
                if (qualitySums[mzLimit] < minQuality)
                {
                    minQuality = qualitySums[mzLimit];
                    minPos = mzLimit;
                }
            }
            currentOptimalQuality[mzAnchor] = minQuality;
            indicesOfOptimalBlockRanges[blockIndex][mzAnchor] = minPos;
        }
    }
}

static inline void ComputePartitionSeparators(Indices& partitionSeparators,
    Matrix<Index>& indicesOfOptimalBlockRanges, unsigned numOfBlocks, unsigned mzCount)
{
    partitionSeparators[0] = 0;
    partitionSeparators[1] = indicesOfOptimalBlockRanges[numOfBlocks - 1][0];
    for (unsigned blockIndex = 2; blockIndex < numOfBlocks + 1; blockIndex++)
    {
        partitionSeparators[blockIndex] =
            indicesOfOptimalBlockRanges[numOfBlocks - blockIndex][
                partitionSeparators[blockIndex - 1]
            ];
    }
    partitionSeparators.back() = mzCount - 1;
}
}
