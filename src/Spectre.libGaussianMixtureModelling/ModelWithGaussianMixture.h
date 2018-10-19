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

#include <fstream>


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

#define CACHE_DATA

#ifdef CACHE_DATA

static void WriteSize(std::ofstream& output_stream, size_t size)
{
    output_stream.write((char*)&size, sizeof(size));
}

static void WriteDataType(std::ofstream& output_stream, DataType data)
{
    output_stream.write((char*)&data, sizeof(DataType));
}

static void WriteTriple(std::ofstream& output_stream,
    const GaussianComponent& component)
{
    WriteDataType(output_stream, component.weight);
    WriteDataType(output_stream, component.mean);
    WriteDataType(output_stream, component.deviation);
}

static void Save(const char* filename, const GaussianMixtureModels& gmms)
{
    std::ofstream output_stream(filename, std::ios::binary);

    WriteSize(output_stream, gmms.size());
    for (unsigned i = 0; i < gmms.size(); i++)
    {
        const GaussianMixtureModel& model = gmms[i];
        WriteSize(output_stream, model.size());
        for (unsigned j = 0; j < model.size(); j++)
        {
            WriteTriple(output_stream, model[j]);
        }
    }
    output_stream.close();
}

#else

static size_t ReadSize(std::ifstream& input_stream)
{
    size_t size;
    input_stream.read((char*)&size, sizeof(size_t));
    return size;
}

static DataType ReadDataType(std::ifstream& input_stream)
{
    DataType data_point;
    input_stream.read((char*)&data_point, sizeof(DataType));
    return data_point;
}

static GaussianComponent ReadTriple(std::ifstream& input_stream)
{
    GaussianComponent component;
    component.weight = ReadDataType(input_stream);
    component.mean = ReadDataType(input_stream);
    component.deviation = ReadDataType(input_stream);

    return component;
}

static void Load(const char* filename, GaussianMixtureModels& gmms)
{
    std::ifstream input_stream(filename, std::ios::binary);

    const size_t gmms_num = ReadSize(input_stream);
    gmms.resize(gmms_num);
    for (unsigned i = 0; i < gmms_num; i++)
    {
        const size_t components_num = ReadSize(input_stream);
        GaussianMixtureModel& model = gmms[i];
        model.resize(components_num);
        for (unsigned j = 0; j < components_num; j++)
        {
            model[j] = ReadTriple(input_stream);
        }
    }

    input_stream.close();
}

#endif

/// <summary>
/// Model signal with Gaussian Mixture.
/// </summary>
/// <param name="spectrum">Spectrum to be modelled.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <returns>Gaussian Mixture Model of the signal</returns>
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

    GaussianMixtureModels splitters(clearPeakIndices.size());

#ifdef CACHE_DATA
    SplitterSegmentExtractor splitterSegmentExtractor;
    for(Index i = 0; i < clearPeakIndices.size(); i++)
    {
        //printf("Processing %dth out of %llu splitter segment\n", i, clearPeakIndices.size());
        const Spectrum splitterSegment =
            splitterSegmentExtractor.ExtractSplitterSegment(peaks, spectrum,
                valleyIndices, clearPeakIndices[i], resolutionCoefficient);
        const DataType clearPeakMz = peaks.mzs[clearPeakIndices[i]];
        splitters[i] = DecomposeSplitterSegment<DynamicProgramming, ExpectationMaximization>(
            splitterSegment, clearPeakMz, resolutionCoefficient, options);
    }
    //Save("cache.txt", splitters);
#else
    Load("cache.txt", splitters);
#endif

    const size_t segmentsNum = clearPeakIndices.size() + 1;
    GaussianMixtureModels segmentModels(segmentsNum);

#ifdef CACHE_DATA
    for(Index i = 0; i < segmentsNum; i++)
    { 
        //printf("Processing %dth out of %llu segment\n", i, segmentsNum);
        const Spectrum segment = ExtractSegment(
            spectrum, peakIndices, clearPeakIndices, i, splitters);
        segmentModels[i] = DecomposeSegment<DynamicProgramming, ExpectationMaximization>(
            segment, resolutionCoefficient, options);
    }
    //Save("cache_gmm.txt", segmentModels);
#else
    Load("cache_gmm.txt", segmentModels);
#endif

    GaussianMixtureModel signalModel;
    signalModel.reserve(splitters.size() * 3 + segmentModels.size() * 5);

    CombineMixtures(segmentModels, spectrum, splitters, clearPeakIndices,
        peakIndices, resolutionCoefficient, options, signalModel);

    Sort(signalModel);

    //std::cout << "Now, you are like me." << std::endl;
    return signalModel;
}
}
