#include <pybind11/pybind11.h>
#include "Spectre.libClassifier/Svm.h"
#include "Spectre.libGaClassifier/ClassifierFactory.h"

namespace py = pybind11;

PYBIND11_MODULE(GeneticAlgorithm, m) {
    py::class_<spectre::supervised::IClassifier> iClassifier(m, "IClassifier");
    iClassifier.def("Fit", &spectre::supervised::IClassifier::Fit, "Fit the data")
        .def("Predict", &spectre::supervised::IClassifier::Predict, "Predict the data")
        .def("clone", &spectre::supervised::IClassifier::clone, "Clone the classifier");

    
    py::class_<spectre::supervised::ClassifierFactory> gaClassifier(m, "ClassifierFactory");
    gaClassifier.def(py::init<>());
        //.def("buildSvm", &spectre::supervised::ClassifierFactory::buildSvm)
        //.def("buildGaClassifier", &spectre::supervised::ClassifierFactory::buildGaClassifier);

    /*
    py::class_<spectre::supervised::Svm> svm(m, "Svm", iClassifier);
    svm.def(py::init<unsigned int, double>())
        .def("Fit", &spectre::supervised::Svm::Fit, "Fit the data")
        .def("Predict", &spectre::supervised::Svm::Predict, "Predict the data")
        .def("clone", &spectre::supervised::Svm::clone, "Clone the classifier")
        .def("GetNumberOfSupportVectors", &spectre::supervised::Svm::GetNumberOfSupportVectors, "Get number of support vectors");
    

    py::class_<spectre::supervised::GaClassifier> gaClassifier(m, "GaClassifier", iClassifier);
    gaClassifier.def(py::init<std::unique_ptr<spectre::supervised::IClassifier>, double, double, double, double, unsigned int, unsigned int, unsigned int,
        spectre::algorithm::genetic::Seed, size_t, size_t>())
        .def("Fit", &spectre::supervised::GaClassifier::Fit, "Fit the data")
        .def("Predict", &spectre::supervised::GaClassifier::Predict, "Predict the data")
        .def("clone", &spectre::supervised::GaClassifier::clone, "Clone the classifier");
    */
}