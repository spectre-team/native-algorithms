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
#include <numeric>
#include <limits>
#include <vector>
#include "Spectre.libException/OutOfRangeException.h"
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/DynamicProgrammingInitialization.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationMaximization.h"
#include "Spectre.libGaussianMixtureModelling/GaussianMixtureModel.h"
#include "Spectre.libGaussianMixtureModelling/Matrix.h"
#include "Spectre.libStatistics/Statistics.h"
#include <omp.h>

namespace spectre::unsupervised::gmm
{
using namespace core::exception;
using namespace core::functional;
using namespace statistics::simple_statistics;
using namespace statistics::basic_math;

typedef std::vector<GaussianComponent> MixtureModel;

static Data InitRightmostBlockQualitiesPar(SpectrumView, DataType, DataType);
static MixtureModel ComputeGaussiansFromBlocksParImpl(SpectrumView, DataType,
    Indices);
Matrix<DataType> ComputeQualityMatrixPar(SpectrumView, DataType, DataType);
Indices ComputeOptimalBlocksPar(SpectrumView, unsigned, Matrix<DataType>&,
    DataType, DataType);
MixtureModel ComputeGaussiansFromBlocksPar(SpectrumView, Indices);
MixtureModel ComputeGaussiansFromBlocksPar(SpectrumView, DataView, Indices);

/// <summary>
/// Initializes gaussian components for ExpectationMaximization algorithm.
/// </summary>
class DynProgInitializationPar
{
public:
    /// <summary>
    /// Initializes the Quality Matrix, later used for block computation.
    /// </summary>
    /// <param name="view">Signal to initialize components of.</param>
    /// <param name="delta">Parameter used for quality function computation
    /// in equation (27).</param>
    /// <param name="minStd">Smallest acceptable standard deviation assigned
    /// to each component.</param>
    DynProgInitializationPar(SpectrumView view, DataType delta,
        DataType minStd)
        : m_QualityOfRange(ComputeQualityMatrixPar(view, delta, minStd))
        , m_Delta(delta)
        , m_MinStd(minStd)
    {
    }

    /// <summary>
    /// Returns the initial parameter set for desired signal decomposition.
    /// </summary>
    /// <param name="view">Signal to initialize components of.</param>
    /// <param name="numOfComponents">Desired number of components</param>
    MixtureModel Initialize(SpectrumView view, unsigned numOfComponents)
    {
        const Indices partitions = ComputeOptimalBlocksPar(view,
            numOfComponents, m_QualityOfRange, m_Delta, m_MinStd);
        return ComputeGaussiansFromBlocksPar(view, partitions);
    }

    /// <summary>
    /// Returns the initial parameter set for desired signal decomposition.
    /// Used for segment decomposition.
    /// </summary>
    /// <param name="view">Signal to initialize components of.</param>
    /// <param name="intensities">Intensities of the segment</param>
    /// <param name="numOfComponents">Desired number of components</param>
    MixtureModel Initialize(SpectrumView view, DataView intensities,
        unsigned numOfComponents)
    {
        const Indices partitions = ComputeOptimalBlocksPar(view,
            numOfComponents, m_QualityOfRange, m_Delta, m_MinStd);
        return ComputeGaussiansFromBlocksPar(view, intensities, partitions);
    }

private:
    Matrix<DataType> m_QualityOfRange;
    const DataType m_Delta;
    const DataType m_MinStd;
};

/// <summary>
/// Computes a basic quality of a block based on given block data. Equation (28)
/// </summary>
/// <param name="blockData">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero.</param>
/// <param name="intensitySum">Sum of intensity values in blockData.</param>
/// <returns>Quality Q1 of the block</returns>
inline DataType ComputeBasicQualityPar(SpectrumView blockData, DataType intensitySum)
{
    const DataView& mzs = blockData.mzs;
    const DataView& intensities = blockData.intensities;
    DataType sumInverse = 1. / intensitySum; // Since * is cheaper than /
    std::vector<DataType> normalizedHeights(mzs.length()); // (16) in the paper
    DataType innerSum = 0;
    int i = 0;
#pragma omp parallel for reduction(+: innerSum) 
    for (i = 0; i < (int)mzs.length(); i++)
    {
        //printf("Thread #%d, iter #%d\n", omp_get_thread_num(), i);
        DataType normalizedHeight = intensities[i] * sumInverse;
        normalizedHeights[i] = normalizedHeight;
        innerSum += mzs[i] * normalizedHeight;
    }
    DataType quality = 0.0;
#pragma omp parallel for reduction(+: quality) 
    for (i = 0; i < (int)mzs.length(); i++)
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
inline  DataType ComputeBlockQualityPar(SpectrumView blockData, DataType delta,
    DataType minStd)
{
    constexpr DataType MIN_SUMMED_HEIGHT = 0.001;
    const DataView mzs = blockData.mzs;
    const DataView intensities = blockData.intensities;

    DataType mzRange = ComputeMzRange(mzs);
    bool failsWidthRequirement = mzRange <= minStd;
    DataType sum = std::accumulate(intensities.begin(), intensities.end(), 0.0);
    bool failsSummedHeightRequirement = sum <= MIN_SUMMED_HEIGHT;

    if (failsSummedHeightRequirement || failsWidthRequirement)
    {
        return std::numeric_limits<DataType>::infinity();
    }

    const DataType indexOneQuality = ComputeBasicQualityPar(blockData, sum);
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
inline Indices ComputeOptimalBlocksPar(SpectrumView spectrum, unsigned numOfBlocks,
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
        std::move(InitRightmostBlockQualitiesPar(spectrum, delta, minStd));
    // currentOptQualityIndex contains Q^opt(1) at this point.
    ComputeOptimalBlockRanges(currentOptimalQuality, qualitySums,
        indicesOfOptimalBlockRanges, qualityOfRange, numOfBlocks);

    Indices partitionSeparators(numOfBlocks + 2); // + 2 for the last and first index.
    ComputePartitionSeparators(partitionSeparators, indicesOfOptimalBlockRanges,
        numOfBlocks, mzCount);
    return partitionSeparators;
}

/// <summary>
/// Computes initial gaussian mixture model parameters, based on provided
/// optimal block intervals.
/// </summary>
/// <param name="spectrum">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero. </param>
/// <param name="blockPartitions">Indices determining partitions.</param>
/// <exception cref="OutOfRangeException">Thrown when number of blocks is
/// greater than signal length - 2.</exception>
/// <returns>List of block ranges. First index symbolises the beginning of the
/// first block, while the last - the end of the last block. Every other index
/// symbolises the end of the block, and the beginning of the next one.</returns>
inline MixtureModel ComputeGaussiansFromBlocksPar(SpectrumView spectrum,
    Indices blockPartitions)
{
    const DataType sum = Sum(spectrum.intensities);
    return ComputeGaussiansFromBlocksParImpl(spectrum, sum, blockPartitions);
}

/// <summary>
/// Computes initial gaussian mixture model parameters, based on provided
/// optimal block intervals.
/// </summary>
/// <param name="spectrum">Spectrum to derive block lengths from. It is assumed
/// that mzs and intensities have same length which is non-zero. </param>
/// <param name="intensities">Intensities for computing height components.
/// </param>
/// <param name="blockPartitions">Indices determining partitions.</param>
/// <exception cref="OutOfRangeException">Thrown when number of blocks is
/// greater than signal length - 2.</exception>
/// <returns>List of block ranges. First index symbolises the beginning of the
/// first block, while the last - the end of the last block. Every other index
/// symbolises the end of the block, and the beginning of the next one.
/// </returns>
inline MixtureModel ComputeGaussiansFromBlocksPar(SpectrumView spectrum,
    DataView intensities, Indices blockPartitions)
{
    const DataType sum = Sum(intensities);
    return ComputeGaussiansFromBlocksParImpl(spectrum, sum, blockPartitions);
}

/// <summary>
/// Computes the qualities of all possible block intervals, and save them to
/// matrix, where matrix(start, end) returns a quality for interval [start, end).
/// </summary>
/// <param name="view">Spectrum to base computation of qualities on.</param>
/// <param name="delta">A positive constant from equation (27).</param>
/// <param name="minStd">Smallest mz range for which calculated
/// range quality attains a positive finite number.</param>
/// <returns>Matrix of qualities as described in the summary.</returns>
inline Matrix<DataType> ComputeQualityMatrixPar(SpectrumView view, DataType delta,
    DataType minStd)
{
    const unsigned mzCount = (unsigned)view.mzs.size();
    Matrix<DataType> qualities(mzCount, mzCount);
    for (unsigned i = 0; i < mzCount - 1; i++)
    {
#pragma omp parallel for
        for (int j = i + 1; j < (int)mzCount; j++)
        {
            qualities[i][j] = ComputeBlockQualityPar(view.subspan(i, j - i), delta, minStd);
        }
    }
    return qualities;
}

static MixtureModel ComputeGaussiansFromBlocksParImpl(SpectrumView spectrum,
    DataType intensitySum, Indices blockPartitions)
{
    int numOfBlocks = (int)blockPartitions.size() - 1;
    MixtureModel components(numOfBlocks);

#pragma omp parallel for
    for (int i = 0; i < numOfBlocks; i++)
    {
        Index blockStart = blockPartitions[i];
        Index blockEnd = blockPartitions[i + 1];
        unsigned blockLength = blockEnd - blockStart;
        SpectrumView block = spectrum.subspan(blockStart, blockLength);
        Data normalizedHeights = Normalize(block.intensities);
        Data scaledMzs = multiplyBy(block.mzs, DataView(normalizedHeights));
        components[i].mean = Sum(DataView(scaledMzs));
        components[i].weight = Sum(block.intensities) / intensitySum;
        components[i].deviation = 0.5 * ComputeMzRange(block.mzs);
    }

    return components;
}

static Data InitRightmostBlockQualitiesPar(SpectrumView spectrum, DataType delta,
    DataType minStd)
{
    const int mzCount = (int)spectrum.mzs.size();
    Data qualities(mzCount);
#pragma omp parallel for
    for (int mzIndex = 0; mzIndex < mzCount; mzIndex++)
    {
        unsigned subSpanSize = mzCount - mzIndex;
        qualities[mzIndex] =
            ComputeBlockQualityPar(spectrum.subspan(mzIndex, subSpanSize), delta, minStd);
    }
    return qualities;
}
}
