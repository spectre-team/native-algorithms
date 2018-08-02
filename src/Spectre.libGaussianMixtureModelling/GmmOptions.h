/*
 * GmmOptions.h
 * Tunable options for Gaussian Mixture Modelling algorithm.
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

namespace spectre::unsupervised::gmm
{
///<summary>
/// Constitutes a set of default, modifyiable options for running Gaussian
/// Mixture Modelling algorithm.
///</summary>
struct GmmOptions
{
    ///<summary>
    /// Multiplied by estimated average width of the peak in the spectrum.
    /// Defines resolutioon of the decomposition.
    ///</summary>
    DataType resolution = 0.5;

    ///<summary>
    /// Determines peak detection sensitivity.
    /// Used for finding split peaks. A height of a single peak must be greater
    /// than the product of this parameter and height of the highest peak.
    ///</summary>
    DataType sensitivity = 0;

    ///<summary>
    /// Determines peak quality threshold.
    /// Used for finding split peaks. Split peaks must have
    /// a higher quality value than this parameter to be accepted.
    ///</summary>
    DataType qualityThreshold = 0.3;

    ///<summary>
    /// Determines an initial offset from peak list during clear peaks search.
    ///</summary>
    Index initialJump = 5;

    ///<summary>
    /// Determines a peak lookup range during clear peaks search.
    ///</summary>
    Index peakLookupRange = 4;

    ///<summary>
    /// Determines a minimal offset from current offset in peak list during clear
    /// peaks search.
    ///</summary>
    Index minJump = 4;

    ///<summary>
    /// Used in quality function during dynamic programming initialization.
    ///</summary>
    DataType qualityFunctionParam = 0.5; // todo(mgallus): find better name.

    ///<summary>
    /// Used in EM iterations to define lower bounds for standard deviation.
    ///</summary>
    DataType minStd = 0.1;

    ///<summary>
    /// Weight used to pick the best gmm decomposition of splitting segments
    /// penalty coefficient for a number of components in the quality funtion.
    /// The number of components is multiplied by this coefficient to obtain
    /// the penalty for using a particular amount of components.
    ///</summary>
    DataType splittingPenaltyCoefficient = 0.0001;

    ///<summary>
    /// Used to determine the minimal amount of iterations to be performed
    /// when searching for most accurate number of gaussian components in
    /// splitter segment decomposition.
    ///</summary>
    Index maxComponentSearchIterations = 15; // todo(mgallus): find better name.

    ///<summary>
    /// Weight used to pick the best gmm decomposition of segments
    /// penalty coefficient for a number of components in the quality funtion.
    ///</summary>
    DataType segmentPenaltyCoefficient = 0.001;

    ///<summary>
    /// Used to determine the max amount of iterations to be performed
    /// when searching for most accurate number of gaussian components in
    /// segment decomposition.
    ///</summary>
    Index minComponentSearchIterations = 30; // todo(mgallus): find better name.

    ///<summary>
    /// Used to determine the max amount of components used to build up a
    /// splitter.
    ///</summary>
    Index maxComponentsNumForSplitter = 10;

    ///<summary>
    /// Used to determine the max amount of components used to build up a
    /// segment.
    ///</summary>
    Index maxComponentsNumForSegment = 30;

    ///<summary>
    /// Tolerance of parameters change (mean, variance, peak height) for
    /// Expectation Maximization.
    ///</summary>
    DataType emEpsilon = 0.0001;
};
}