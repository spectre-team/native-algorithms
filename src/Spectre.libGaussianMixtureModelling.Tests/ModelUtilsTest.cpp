/*
* ModelUtilsTest.cpp
* Tests the functionality of gaussian mixture model utilities.
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
#define GTEST_LANG_CXX11 1

#include <gtest/gtest.h>
#include "Spectre.libGaussianMixtureModelling/ModelUtils.h"

namespace spectre::unsupervised::gmm
{
class ModelUtilsTest : public ::testing::Test
{
public:
    ModelUtilsTest()
    {
        data.mzs = { 0.0, 2.0, 4.0, 6.0, 8.0 };
        data.intensities = { 0.0, 2.0, 6.0, 2.0, 0.0 };
        model.push_back({ 4.0, 0.5, 1.0 });
    }

protected:
    Spectrum data;
    GaussianMixtureModel model;
    static constexpr DataType MAX_ERROR = 0.0001;
};

TEST_F(ModelUtilsTest, correctly_computes_scale)
{
    Data mixture(data.mzs.size());
    for(unsigned i = 0; i < mixture.size(); i++)
    {
        mixture[i] = model[0].weight *
            Gaussian(data.mzs[i], model[0].mean, model[0].deviation);
    }

    DataType scale = ComputeModelScale(data.intensities, mixture);
    ASSERT_NEAR(12.524738209967269, scale, MAX_ERROR);
}

TEST_F(ModelUtilsTest, correctly_compues_model_intensities)
{
    Data correct = {
        1.0104542167073785e-14,
        0.00026766045152977074,
        0.79788456080286541,
        0.00026766045152977074,
        1.0104542167073785e-14,
    };

    Data intensities = ComputeIntensitiesFor(data.mzs, model);

    for(unsigned i = 0; i < intensities.size(); i++)
    {
        ASSERT_NEAR(correct[i], intensities[i], MAX_ERROR);
    }
}

TEST_F(ModelUtilsTest, correctly_scales_components)
{
    const DataType scale = 2.0;
    const DataType correct = model[0].weight * scale;
    GaussianMixtureModel modelCopy(model);
    ScaleComponents(modelCopy, scale);
    ASSERT_NEAR(correct, modelCopy[0].weight, MAX_ERROR);
}
}
