// Auto-generated file. Do not edit!
//   Template: src/f32-vlrelu/sse.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <xmmintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/vunary.h>


void xnn_f32_vlrelu_ukernel__sse_x4(
    size_t n,
    const float* x,
    float* y,
    const union xnn_f32_lrelu_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);

  const __m128 vslope = _mm_load_ps(params->sse.slope);
  const __m128 vzero = _mm_setzero_ps();

  for (; n >= 4 * sizeof(float); n -= 4 * sizeof(float)) {
    const __m128 vx0123 = _mm_loadu_ps(x);
    x += 4;

    __m128 vacc0123 = _mm_mul_ps(_mm_min_ps(vx0123, vzero), vslope);

    vacc0123 = _mm_add_ps(vacc0123, _mm_max_ps(vx0123, vzero));

    _mm_storeu_ps(y, vacc0123);
    y += 4;
  }
  if XNN_UNLIKELY(n != 0) {
    const __m128 vx = _mm_loadu_ps(x);
    __m128 vacc = _mm_mul_ps(_mm_min_ps(vx, vzero), vslope);
    vacc = _mm_add_ps(vacc, _mm_max_ps(vx, vzero));

    if (n & (2 * sizeof(float))) {
      _mm_storel_pi((__m64*) y, vacc);
      vacc = _mm_movehl_ps(vacc, vacc);
      y += 2;
    }
    if (n & (1 * sizeof(float))) {
      _mm_store_ss(y, vacc);
    }
  }
}
