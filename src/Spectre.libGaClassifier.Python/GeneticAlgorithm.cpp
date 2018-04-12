#include <pybind11/pybind11.h>
#include "Spectre.libClassifier/IClassifier.h"
#include "Spectre.libClassifier/DatasetFactory.h"
#include "Spectre.libGaClassifier/ClassifierFactory.h"

namespace py = pybind11;

namespace spectre::supervised {
PYBIND11_MODULE(genetic_algorithm, m) {
    py::class_<IClassifier> iClassifier(m, "IClassifier");
    iClassifier.def("Fit", &IClassifier::Fit, "Fit the data")
        .def("Predict", &IClassifier::Predict, "Predict the data")
        .def("clone", &IClassifier::clone, "Clone the classifier");

    py::class_<DatasetFactory> datasetFactory(m, "DatasetFactory");
    datasetFactory.def(py::init<>())
        .def("create", &DatasetFactory::create, "create openCvDataset");

    py::class_<ClassifierFactory> gaClassifier(m, "ClassifierFactory");
    gaClassifier.def(py::init<>())
        .def("buildSvm", &ClassifierFactory::buildSvm)
        .def("buildGaClassifier", &ClassifierFactory::buildGaClassifier);
}
}