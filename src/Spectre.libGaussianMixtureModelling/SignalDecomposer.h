#pragma once
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "Spectre.libGaussianMixtureModelling/DynamicProgrammingInitialization.h"
#include "Spectre.libGaussianMixtureModelling/ExpectationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/GmmOptions.h"
#include "Spectre.libGaussianMixtureModelling/MaximizationRunnerOpt.h"
#include "Spectre.libGaussianMixtureModelling/ModelUtils.h"
#include "Spectre.libFunctional/Range.h"
#include "Spectre.libStatistics/Statistics.h"
#include "Spectre.libStatistics/Types.h"

namespace spectre::unsupervised::gmm
{
using namespace statistics::basic_math;
using namespace statistics::simple_statistics;
using namespace core::functional;

static SpectrumView CompressDataIfNecessary(SpectrumView, Spectrum&);
static GaussianComponent ComputeComponentForASingleBlock(SpectrumView);
static GaussianMixtureModel DecomposeSplitterSignal(SpectrumView,
    SpectrumView, const GmmOptions&, DataType, DataType, int, int);
static GaussianMixtureModel DecomposeSegment(SpectrumView, SpectrumView,
    const GmmOptions&, DataType, DataType, int, int);

/// <summary>
/// Performs initialization for Gaussian Mixture Modelling Expectation Maximization
/// algorithm.
/// </summary>
/// <param name="segment">Signal to perform initialization for.</param>
/// <param name="penaltyCoefficient">Used for limiting number of components.</param>
/// <param name="resolutionCoefficient">Signal resolution.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <param name="minNumOfBlocks">Minimal number of blocks to approximate
/// a segment (1) or splitter segment (2).</param>
/// <param name="decomposeSplitter">Set to true if splitter-segment is decomposed.
/// </param>
/// <returns>Initial parameters in a form of Gaussian Mixture Model.</returns>
template<typename DynamicProgramming, typename ExpectationMaximization>
inline GaussianMixtureModel DecomposeSignal(SpectrumView segment,
    DataType penaltyCoefficient, DataType resolutionCoefficient, 
    const GmmOptions& options, int minNumOfBlocks, bool decomposeSplitter)
{
    const unsigned mzCount = (unsigned)segment.mzs.size();
    Spectrum compressedSegment;
    SpectrumView processedData = CompressDataIfNecessary(segment, compressedSegment);
    DataType minStd = options.minStd * resolutionCoefficient * Mean(segment.mzs);
    const int approxNumOfBlocks = (int)floor(ComputeMzRange(segment.mzs) / minStd);
    int startNumOfBlocks = std::min(minNumOfBlocks, approxNumOfBlocks - 1);
    if (startNumOfBlocks <= 0)
    {
        return { ComputeComponentForASingleBlock(segment) };
    }
    if (decomposeSplitter)
    {
        return DecomposeSplitterSignal<DynamicProgramming, ExpectationMaximization>(
            segment, processedData, options, minStd, penaltyCoefficient,
            approxNumOfBlocks, startNumOfBlocks);
    }
    else
    {
        return DecomposeSegment<DynamicProgramming, ExpectationMaximization>(segment,
            processedData, options, minStd, penaltyCoefficient, approxNumOfBlocks,
            startNumOfBlocks);
    }
}

// Resample intensities. This has some strong assumptions, so if anything goes wrong
// when testing on real data, then perhaps revise this. It currently assumes that
// if oldMz_i > resampledMz_j, then the next resampledMz (j+1) has to be greater
// than oldMz_i. Not sure if that's always true.
static Data BinData(DataView newMzs, DataView oldMzs, DataView oldIntensities)
{
    const unsigned oldMzCount = (unsigned)oldMzs.size();
    const unsigned newMzCount = (unsigned)newMzs.size() - 1;
    Indices mzAffilation(oldMzCount);
    Indices affilationCounts(newMzCount);
    Index lastValidIndex = newMzCount - 1;
    for (Index i = 0, j = 0; i < oldMzCount; i++)
    {
        if (oldMzs[i] < newMzs[j + 1])
            mzAffilation[i] = j;
        else
        {
            j = std::min(j + 1, lastValidIndex);
            mzAffilation[i] = j;
        }
        affilationCounts[j]++;
    }

    Data newIntensities(newMzCount);
    for (Index i = 0; i < oldMzCount; i++)
    {
        newIntensities[mzAffilation[i]] += oldIntensities[i];
    }

    for (Index i = 0; i < newMzCount; i++)
    {
        newIntensities[i] /= (DataType)affilationCounts[i];
    }

    return newIntensities;
}

template<class ForwardIt, class UnaryPredicate>
ForwardIt RemoveIf(ForwardIt first_int, ForwardIt first_mz,
    ForwardIt last, ForwardIt& result_mz, UnaryPredicate p)
{
    auto temp = std::find_if(first_int, last, p);
    first_mz = first_mz + std::distance(first_int, temp);
    first_int = temp;
    if (first_int != last)
        for (ForwardIt i = first_int, j = first_mz; ++j, ++i != last;)
            if (!p(*i))
            {
                *first_int++ = std::move(*i);
                *first_mz++ = std::move(*j);
            }
    result_mz = first_mz;
    return first_int;
}

static void FilterNansOut(Spectrum& signal)
{
    Data::iterator mzEnd;
    Data::iterator intensitiesEnd = RemoveIf(signal.intensities.begin(),
        signal.mzs.begin(), signal.intensities.end(), mzEnd,
        [](DataType x) -> bool { return isnan(x); });
    signal.mzs.erase(mzEnd, signal.mzs.end());
    signal.intensities.erase(intensitiesEnd, signal.intensities.end());
}

// It assumes that BinData returns no NaNs.
static SpectrumView CompressSignal(SpectrumView signal,
    Spectrum& compressedSignal)
{
    const unsigned COMPRESSED_RANGE = 200;
    DataType step = ComputeMzRange(signal.mzs) / COMPRESSED_RANGE;
    compressedSignal.mzs = range(*signal.mzs.begin(), *(signal.mzs.end() - 1), step);
    compressedSignal.mzs = plus(DataView(compressedSignal.mzs), 0.5 * step);
    compressedSignal.intensities = BinData(compressedSignal.mzs, signal.mzs,
        signal.intensities);
    FilterNansOut(compressedSignal);
    SpectrumView view(compressedSignal);
    return view;
}

static SpectrumView CompressDataIfNecessary(SpectrumView segment,
    Spectrum& compressedSegment)
{
    const unsigned MAX_UNCOMPRESSED_SIGNAL_LENGTH = 300;
    const unsigned mzCount = (unsigned)segment.mzs.size();
    if (mzCount > MAX_UNCOMPRESSED_SIGNAL_LENGTH)
    {
        return CompressSignal(segment, compressedSegment);
    }
    else
    {
        return segment;
    }
}

static GaussianComponent ComputeComponentForASingleBlock(SpectrumView segment)
{
    Data normalizedHeights = Normalize(segment.intensities);
    GaussianComponent component;
    Data scaledMzs = multiplyBy(segment.mzs, DataView(normalizedHeights));
    component.mean = Sum(DataView(scaledMzs));
    component.weight = 1.0;
    Data sampleMinusMeanVector = minus(segment.mzs, component.mean);
    Data sampleMinusMeanSquared =
        multiplyBy(DataView(sampleMinusMeanVector), DataView(sampleMinusMeanVector));
    Data squaredTimesNormHeights =
        multiplyBy(DataView(sampleMinusMeanSquared), DataView(normalizedHeights));
    component.deviation = sqrt(Sum(DataView(squaredTimesNormHeights)));

    return component;
}

static DataType ComputeModelQuality(DataView intensities, DataView mixture,
    DataType scale)
{
    Data scaledIntensities = divideBy(intensities, scale);
    Data approximationErrors = minus(DataView(scaledIntensities), mixture);
    Data moduleOfErrors = abs(DataView(approximationErrors));
    return Sum(DataView(moduleOfErrors)) / Sum(mixture);
}

template<typename DynamicProgramming, typename ExpectationMaximization>
static GaussianMixtureModel DecomposeSplitterSignal(SpectrumView original,
    SpectrumView processed, const GmmOptions& options, DataType minStd,
    DataType penaltyCoefficient, int approxNumOfBlocks, int startNumOfBlocks)
{
    GaussianMixtureModel bestComponents;
    const DataType delta = options.qualityFunctionParam;
    DataType scale = 0.0;
    DynProgInitialization init(processed, delta, minStd);
    DataType minApproximationQuality = std::numeric_limits<DataType>::infinity();
    const unsigned maxViableBlocks =
        std::min(options.minComponentSearchIterations, (unsigned)approxNumOfBlocks);
    const unsigned maxNumOfBlocks = 2 * (startNumOfBlocks + maxViableBlocks);
    auto expectationMaximization = ExpectationMaximization(original, maxNumOfBlocks);
    for (unsigned blockNum = startNumOfBlocks; blockNum < maxNumOfBlocks; blockNum++) 
    {
        GaussianMixtureModel components = init.Initialize(processed, blockNum);
        expectationMaximization.EstimateGmm(original, components, minStd,
            options.emEpsilon);
        const Data mixture = ComputeIntensitiesFor(original.mzs, components);
        scale = ComputeModelScale(original.intensities, mixture);
        const DataType quality = ComputeModelQuality(original.intensities, mixture,
            scale);
        const DataType penalisedQuality = quality + penaltyCoefficient * blockNum;
        if (penalisedQuality < minApproximationQuality)
        {
            minApproximationQuality = penalisedQuality;
            bestComponents = std::move(components);
        } // If the quality no longer improves, and we're beyond viable num of blocks,
        else if ((blockNum - startNumOfBlocks) > maxViableBlocks) // - terminate.
        {
            break;
        }
    }
    ScaleComponents(bestComponents, scale);
    return bestComponents;
}

template<typename DynamicProgramming, typename ExpectationMaximization>
static GaussianMixtureModel DecomposeSegment(SpectrumView original,
    SpectrumView processed, const GmmOptions& options, DataType minStd,
    DataType penaltyCoefficient, int approxNumOfBlocks, int startNumOfBlocks)
{
    if(original.mzs.size() < 3)
    {
        return GaussianMixtureModel();
    }
    GaussianMixtureModel bestComponents;
    const DataType delta = options.qualityFunctionParam;
    DataType scale = 0.0;
    DynProgInitialization init(processed, delta, minStd);
    DataType minApproximationQuality = std::numeric_limits<DataType>::infinity();
    const unsigned maxViableBlocks = std::min((unsigned)original.mzs.size() / 4u,
        std::min(options.minComponentSearchIterations, (unsigned)approxNumOfBlocks));
    const unsigned maxNumOfBlocks = options.maxComponentsNumForSegment;
    auto expectationMaximization = ExpectationMaximization(original, maxNumOfBlocks);
    GaussianMixtureModel components;
    for (unsigned blockNum = startNumOfBlocks; blockNum < maxNumOfBlocks; blockNum++)
    {
        components =
            init.Initialize(processed, original.intensities, blockNum);
        expectationMaximization.EstimateGmm(original, components, minStd,
            options.emEpsilon);
        const Data mixture = ComputeIntensitiesFor(original.mzs, components);
        scale = ComputeModelScale(original.intensities, mixture);
        const DataType quality = ComputeModelQuality(original.intensities, mixture,
            scale);
        const DataType penalisedQuality = quality + penaltyCoefficient * blockNum;
        if (penalisedQuality < minApproximationQuality)
        {
            minApproximationQuality = penalisedQuality;
            bestComponents = std::move(components);
        } // If the quality no longer improves, and we're beyond viable num of blocks,
        else if ((blockNum - startNumOfBlocks) > maxViableBlocks) // - terminate.
        {
            components = std::move(bestComponents);
            break;
        }
    } // It is unsure if the author of the ref version really meant returning
    ScaleComponents(components, scale); // components and not bestComponents here.
    return components;
}
}
