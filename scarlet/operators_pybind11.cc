#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <algorithm>

namespace py = pybind11;

void prox_monotonic(
  // Fast implementation of monotonicity constraint
  py::array_t<double> &X,
  double const &step,
  std::vector<bool> const &seeks,
  std::vector<int> const &ref_idx,
  std::vector<int> const &dist_idx,
  double const &thresh
){
  auto x = X.mutable_unchecked<2>();
  for(unsigned long pk=0; pk<x.shape(0); pk++){
    // only apply to those components in seeks
    if (seeks[pk]) {
      // Start at the center of the image and set each pixel to the minimum
      // between itself and its reference pixel (which is closer to the peak)
      for(auto &didx: dist_idx){
        x(pk, didx) = std::min(x(pk, didx), x(pk, ref_idx[didx])-thresh);
      }
    }
  }
}

PYBIND11_PLUGIN(operators_pybind11)
{
  py::module mod("operators_pybind11", "Fast proximal operators");
  mod.def("prox_monotonic", &prox_monotonic, "Monotonic Proximal Operator");
  return mod.ptr();
}
