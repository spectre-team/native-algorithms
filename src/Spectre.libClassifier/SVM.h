﻿/*
* SVM.h
* 
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

#pragma once
#include <opencv2/ml/ml.hpp>
#include "Spectre.libClassifier/SplittedOpevCvDataset.h"
#include "Spectre.libClassifier/PredictionResultsMatrix.h"

class SVM
{
public:
    SVM();
    Spectre::libClassifier::PredictionResultsMatrix getResult(Spectre::libClassifier::SplittedOpenCvDataset&& data) const;
private:
    cv::Ptr<cv::ml::SVM> mSVM;

    void train(Spectre::libClassifier::OpenCvDataset data) const;
    Spectre::libClassifier::PredictionResultsMatrix predict(Spectre::libClassifier::OpenCvDataset data) const;
};
