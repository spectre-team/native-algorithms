/*
* ModelUtils.h
* Functions for operating on GaussianMixtureModel construct.
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
#include "Spectre.libStatistics/Statistics.h"
#include "Spectre.libGaussianMixtureModelling/DataTypes.h"
#include "GaussianDistribution.h"

namespace spectre::unsupervised::gmm
{
using statistics::simple_statistics::Sum;

/// <summary>Computes the scale coefficient of the spectrum. The return
/// value can be multiplied by model heights to obtain heights that would
/// correspond to singals actual size.</summary>
/// <param name="intensities">Intensities of the signal</param>
/// <param name="mixture">Intensities of the model</param>
/// <returns>Scale coefficient.</returns>
inline DataType ComputeModelScale(DataView intensities, DataView mixture)
{
    return Sum(intensities) / Sum(mixture);
}

/// <summary>Computes intensities for a model across given mzs.</summary>
/// <param name="mzs">Mzs to compute intensities for.</param>
/// <param name="mixture">Model to estimate intensities from.</param>
/// <returns>Intensities of the model.</returns>
inline Data ComputeIntensitiesFor(DataView mzs,
    const GaussianMixtureModel& components)
{
    Data mixture(mzs.size());
    for (Index i = 0; i < components.size(); i++)
    {
        for (Index j = 0; j < mixture.size(); j++)
        {
            mixture[j] += components[i].weight *
                Gaussian(mzs[j], components[i].mean, components[i].deviation);
        }
    }
    return mixture;
}

/// <summary>Scales heights of the components by a given coefficient.</summary>
/// <param name="components">Components with heights to be scaled.</param>
/// <param name="scale">Scale coefficient.</param>
inline void ScaleComponents(GaussianMixtureModel& components, DataType scale)
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        components[i].weight *= scale;
    }
}
}
