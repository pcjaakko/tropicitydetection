#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "geometry3.hh"
#include "cube.hh"
#include "trajectory.hh"

namespace py = pybind11;

PYBIND11_PLUGIN(libtropicity) {
  py::module m("lalala", "pybind11 ... whatever");

// class cube
  py::class_<Cube>(m, "cube")
    .def(py::init<string>());

// // class canvas
//   py::class_<canvas>(m, "canvas")
//     .def(py::init<const string , int , int>())
//     .def("bucket_fill", &canvas::bucket_fill) // int, int, int
//     .def("render_scene", &canvas::render_scene) // scene
//     .def("highlight_edges", &canvas::highlight_edges)
//     .def("draw_coast", &canvas::draw_coast) // scene
//     .def("annotate_peaks", &canvas::annotate_peaks) // scene
//     .def("annotate_islands", &canvas::annotate_islands) // scene
//     .def("label_axis", &canvas::label_axis); // scene

    return m.ptr();
}

