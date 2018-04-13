#include <pybind11/pybind11.h>
#include "Spectre.libClassifier/IClassifier.h"
#include "Spectre.libClassifier/DatasetFactory.h"
#include "Spectre.libGaClassifier/ClassifierFactory.h"
#include "Spectre.libClassifier/DownsampledOpenCVDataset.h"

namespace py = pybind11;

int add(int a, int b)
{
    return a + b;
}

namespace spectre::supervised {
PYBIND11_MODULE(genetic_algorithm, m) {
    py::class_<IClassifier> iClassifier(m, "IClassifier");
    iClassifier.def("Fit", &IClassifier::Fit, "Fit the data")
        .def("Predict", &IClassifier::Predict, "Predict the data")
        .def("clone", &IClassifier::clone, "Clone the classifier");

    py::class_<Svm> svm(m, "Svm");
    svm.def(py::init<unsigned int, double>());

    py::class_<GaClassifier> gaClassifier(m, "GaClassifier");

    py::class_<DatasetFactory> datasetFactory(m, "DatasetFactory");
    //datasetFactory.def(py::init<>())
    //    .def("create", &DatasetFactory::create, "create openCvDataset");

    py::class_<ClassifierFactory> classifierFactory(m, "ClassifierFactory");
    //gaClassifier.def(py::init<>())
    //    .def("buildSvm", &ClassifierFactory::buildSvm)
    //    .def("buildGaClassifier", &ClassifierFactory::buildGaClassifier);

    //py::class_<cv::Mat> mat(m, "Mat");
    py::class_<ObservationExtractor> observationExtractor(m, "ObservationExtractor");
    py::class_<core::dataset::IReadOnlyDataset<Observation, Label, core::dataset::Empty>> iReadOnlyDataset(m, "IReadOnlyDataset");
    //py::class_<OpenCvDataset> openCvDataset(m, "OpenCvDataset", iReadOnlyDataset);
    //py::class_<SplittedOpenCvDataset> splittedOpenCvDataset(m, "SplittedOpenCvDataset");

    //py::class_<DownsampledOpenCVDataset> downsampledOpenCVDataset(m, "DownsampledOpenCVDataset");
}

PYBIND11_MODULE(test, m) {
    m.def("add", &add, "A function which adds two numbers");
}
}