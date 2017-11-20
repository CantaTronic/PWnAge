
#pragma once

#define PWA_OFFLOADED
#define PWA_START_OFFLOAD
#define PWA_END_OFFLOAD

#ifdef __INTEL_COMPILER
#ifndef PWA_DISABLE_OFFLOAD

#undef PWA_OFFLOADED
#define PWA_OFFLOADED __declspec(target(mic))

#undef PWA_START_OFFLOAD
#define PWA_START_OFFLOAD _Pragma("offload_attribute(push, target(mic))")

#undef PWA_END_OFFLOAD
#define PWA_END_OFFLOAD _Pragma("offload_attribute(pop)")

#endif
#endif
