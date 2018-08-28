/*
* ModelWithGaussianMixture.h
* Provides reference implementation of modelling a signal with
* Gaussian Mixture.
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
#include "Spectre.libGaussianMixtureModelling/GmmOptions.h"
#include "Spectre.libPeakFinder/ExtremaFinder.h"
#include "Spectre.libPeakFinder/FWHHCalculator.h"
#include "Spectre.libPeakFinder/PeakQualityCalculator.h"
#include "Spectre.libWavelet/DaubechiesFiltersDenoiser.h"
#include "Spectre.libStatistics/Statistics.h"
#include "Spectre.libFunctional/Filter.h"
#include "Spectre.libGaussianMixtureModelling/ClearPeakFinder.h"
#include "Spectre.libGaussianMixtureModelling/SplitterSegmentExtractor.h"
#include "Spectre.libGaussianMixtureModelling/SplitterSegmentDecomposer.h"
#include "Spectre.libGaussianMixtureModelling/SegmentExtractor.h"
#include "Spectre.libGaussianMixtureModelling/SegmentDecomposer.h"
#include "EmergencyCorrection.h"


namespace spectre::unsupervised::gmm
{
static DataType ComputeResolutionCoefficient(DataType resolutionParam,
    DataView leftFWHH, DataView rightFWHH, DataView peakMzs)
{
    using namespace statistics::basic_math;
    using namespace statistics::simple_statistics;  
    
    Data peakWidths = minus(rightFWHH, leftFWHH);
    Data scaledWidths = divideBy(DataView(peakWidths), DataView(peakMzs));
    const DataType mean = Mean(DataView(scaledWidths));
    const DataType resolution = resolutionParam * mean;
    return resolution;
}

// Branchless cache-efficient zero filter
static inline Indices GetNonZeros(const GaussianMixtureModel& model)
{
    const size_t length = model.size();
    std::vector<unsigned> indices(length + 1);
    unsigned zeros = 0;

    for (unsigned i = 0; i < length; i++)
    {
        bool dataIsZero = model[i].weight <= 0.0;
        int index = i - zeros + dataIsZero;
        zeros += dataIsZero;

        indices[index] = i;
    }
    indices.resize(length - zeros);

    return indices;
}
using GaussianMixtureModels = std::vector<GaussianMixtureModel>;
void CombineMixtures(GaussianMixtureModels& segmentModels, Spectrum& spectrum,
    GaussianMixtureModels& splitters, Indices& clearPeakIndices,
    Indices& peakIndices, DataType resolutionCoefficient,
    const GmmOptions& options, GaussianMixtureModel& signalModel)
{
    for (Index i = 0; i < segmentModels.size(); i++)
    {
        GaussianMixtureModel& segmentModel = segmentModels[i];
        Indices posHeightIndices = GetNonZeros(segmentModel);
        if (posHeightIndices.empty())
        {
            EmergencyCorrection(i, spectrum, splitters, clearPeakIndices,
                peakIndices, resolutionCoefficient, options);
        }
        else
        {
            signalModel.insert(
                signalModel.end(), segmentModel.begin(), segmentModel.end());
        }
    }

    for (Index i = 0; i < splitters.size(); i++)
    {
        signalModel.insert(
            signalModel.end(), splitters[i].begin(), splitters[i].end());
    }
}

/// <summary>
/// TODO: write
/// </summary>
/// <param name="segment">Splitter segment containing the splitter.</param>
/// <param name="resolutionCoefficient">Signal resolution.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <returns>Gaussian Mixture Model of the segment.</returns>
template<typename SplitterSegmentExtractor, typename ExpectationMaximization,
    typename DynamicProgramming>
inline GaussianMixtureModel ModelWithGaussianMixture(Spectrum& spectrum,
    const GmmOptions& options)
{
    using namespace algorithm::wavelet;
    using namespace algorithm::peakfinder;
    using namespace core::functional;
    using Valleys = Peaks;

    DaubechiesFiltersDenoiser denoiser;
    Data denoised = denoiser.Denoise(spectrum.intensities);

    ExtremaFinder extremaFinder;
    Indices peakIndices = extremaFinder.FindPeaks(denoised);
    Indices valleyIndices = extremaFinder.FindValleys(denoised);

    FWHHCalculator fwhhCalculator;
    Data leftFWHH = fwhhCalculator.GetLeftFWHH(spectrum.mzs,
        denoised, valleyIndices, peakIndices);
    Data rightFWHH = fwhhCalculator.GetRightFWHH(spectrum.mzs,
        denoised, valleyIndices, peakIndices);

    Peaks peaks;
    peaks.mzs = filter(DataView(spectrum.mzs), IndicesView(peakIndices));
    peaks.intensities =
        filter(DataView(denoised), IndicesView(peakIndices));

    valleyIndices =
        extremaFinder.FindValleys(spectrum.intensities, peakIndices);
    Valleys valleys;
    valleys.intensities =
        filter(DataView(spectrum.intensities), IndicesView(valleyIndices));

    DataType resolutionCoefficient = ComputeResolutionCoefficient(
        options.resolution, leftFWHH, rightFWHH, peaks.mzs);

    resolutionCoefficient = 0.00214166770575118;

    PeakQualityCalculator peakQualityCalculator;
    Data peakQualities = peakQualityCalculator.CalculatePeakQualities(
        peaks.intensities, valleys.intensities);

    ClearPeakFinder clearPeakFinder;
    Indices clearPeakIndices = clearPeakFinder.FindClearPeaks(
        peaks, peakQualities, options, resolutionCoefficient);

    SplitterSegmentExtractor splitterSegmentExtractor;
    GaussianMixtureModels splitters(clearPeakIndices.size());
    for(Index i = 0; i < clearPeakIndices.size(); i++)
    {
        const Spectrum splitterSegment =
            splitterSegmentExtractor.ExtractSplitterSegment(peaks, spectrum,
                valleyIndices, clearPeakIndices[i], resolutionCoefficient);
        const DataType clearPeakMz = peaks.mzs[clearPeakIndices[i]];
        splitters[i] = DecomposeSplitterSegment<DynamicProgramming, ExpectationMaximization>(
            splitterSegment, clearPeakMz, resolutionCoefficient, options);
    }

    const size_t segmentsNum = clearPeakIndices.size() + 1;
    GaussianMixtureModels segmentModels(segmentsNum);
    for(Index i = 0; i < segmentsNum; i++)
    {
        const Spectrum segment = ExtractSegment(
            spectrum, peakIndices, clearPeakIndices, i, splitters);
        segmentModels[i] = DecomposeSegment<DynamicProgramming, ExpectationMaximization>(
            segment, resolutionCoefficient, options);
    }

    GaussianMixtureModel signalModel;
    signalModel.reserve(splitters.size() * 3 + segmentModels.size() * 5);

    CombineMixtures(segmentModels, spectrum, splitters, clearPeakIndices,
        peakIndices, resolutionCoefficient, options, signalModel);

    Sort(signalModel);

    return signalModel;
}
}
