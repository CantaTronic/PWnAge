
#pragma once

#include "OffloadDirectives.h"
#include "TFloat.h"

PWA_START_OFFLOAD
#include <complex>
// using cfloat = std::complex<TFloat>;
typedef std::complex<TFloat> cfloat;
PWA_END_OFFLOAD
