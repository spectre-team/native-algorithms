#pragma once
#include "Spectre.libGaussianMixtureModelling/SignalDecomposer.h"

namespace spectre::unsupervised::gmm
{
/// <summary>
/// Takes a segment and returns its decomposition in a form of a gaussian
/// mixture model.
/// </summary>
/// <param name="segment">Splitter segment containing the splitter.</param>
/// <param name="resolutionCoefficient">Signal resolution.</param>
/// <param name="options">GaussianMixtureModelling options set.</param>
/// <returns>Gaussian Mixture Model of the segment.</returns>
inline MixtureModel DecomposeSegment(SpectrumView segment,
    DataType resolutionCoefficient, GmmOptions& options)
{
    constexpr int minNumOfBlocks = 1;
    const DataType penaltyCoefficient = options.segmentPenaltyCoefficient;
    const bool decomposeSplitter = false;
    return DecomposeSignal(segment, penaltyCoefficient,
        resolutionCoefficient, options, minNumOfBlocks, decomposeSplitter);
}
}