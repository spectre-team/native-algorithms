﻿/*
* Svm.cpp
* Simple wrapper for OpenCV SVM.
*
Copyright 2017 Spectre Team

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

#include "Svm.h"
#include "Spectre.libClassifier/OpenCvDataset.h"
#include "Spectre.libClassifier/UnsupportedDatasetTypeException.h"
#include "Spectre.libClassifier/UntrainedClassifierException.h"
#include "InsufficientDataException.h"

namespace spectre::supervised
{
Svm::Svm(unsigned int iterationsLimit, double tolerance)
{
    m_Svm = cv::ml::SVM::create();
    m_Svm->setType(cv::ml::SVM::C_SVC);
    m_Svm->setKernel(cv::ml::SVM::LINEAR);
    const auto termination = cv::TermCriteria(cv::TermCriteria::MAX_ITER, iterationsLimit, tolerance);
    m_Svm->setTermCriteria(termination);
}

void Svm::Fit(LabeledDataset dataset)
{
    auto sum = 0u;
    const auto positive = 1u;
    const auto negative = 0u;
    for (size_t i = 0u; i < dataset.size(); ++i)
    {
        const auto label = dataset.GetSampleMetadata(i);
        if (label != negative && label != positive)
        {
            throw exception::NotABinaryLabelException(label, i, "dataset");
        }
        sum += label;
    }
    if (sum == 0u || sum == dataset.size())
    {
        throw exception::InsufficientDataException(sum == 0u ? positive : negative);
    }
    const auto& data = asSupported(dataset);
    m_Svm->clear();
    m_Svm->train(data.getMatData(), cv::ml::ROW_SAMPLE, data.getMatLabels());
}

std::vector<Label> Svm::Predict(LabeledDataset dataset) const
{
    if (!m_Svm->isTrained())
    {
        throw exception::UntrainedClassifierException();
    }
    const auto& data = asSupported(dataset);
    const auto numberOfObservations = static_cast<unsigned>(data.getMatData().rows);
    std::vector<float> predictions(numberOfObservations, 0);
    cv::Mat predictionOutput(numberOfObservations, 1, CV_TYPE, predictions.data());
    m_Svm->predict(data.getMatData(), predictionOutput);
    std::vector<Label> labels(numberOfObservations, 0);
    cv::Mat labelsOutput(numberOfObservations, 1, CV_LABEL_TYPE, labels.data());
    predictionOutput.convertTo(labelsOutput, CV_LABEL_TYPE);
    return labels;
}

std::unique_ptr<IClassifier> Svm::clone() const
{
    return std::make_unique<Svm>(std::move(Svm(*this)));
}

unsigned int Svm::GetNumberOfSupportVectors() const
{
    return m_Svm->getSupportVectors().rows;
}


const OpenCvDataset& Svm::asSupported(LabeledDataset dataset)
{
    try
    {
        const auto& casted = dynamic_cast<const OpenCvDataset&>(dataset);
        return casted;
    }
    catch (const std::bad_cast&)
    {
        throw exception::UnsupportedDatasetTypeException(dataset);
    }
}
}
