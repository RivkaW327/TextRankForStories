 
#include "text_ranker.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


PYBIND11_MODULE(textranker, m) {
    py::class_<TextRanker>(m, "TextRanker")
        .def(py::init<>())
        .def(py::init<double, int, double >())
        .def("ExtractKeyParagraphs", &TextRanker::ExtractKeyParagraphs,
            "A function that takes a chapter and characters and returns the K most important paragraphs in the chapter");
    //py::class_<Paragraph>(m, "Paragraph")
    //    .def(py::init<>())
    //    .def("GetText", &Paragraph::, "return the text of the paragraph");
};
