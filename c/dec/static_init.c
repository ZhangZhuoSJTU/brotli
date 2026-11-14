/* Copyright 2025 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

#include "static_init.h"

#include "../common/platform.h"
#include "../common/static_init.h"

#if (BROTLI_STATIC_INIT != BROTLI_STATIC_INIT_NONE)
#include "../common/dictionary.h"
#include "prefix.h"
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if (BROTLI_STATIC_INIT != BROTLI_STATIC_INIT_NONE)
static BROTLI_BOOL DoBrotliDecoderStaticInit(void) {
  BROTLI_BOOL ok = BrotliDecoderInitCmdLut(kCmdLut);
  if (!ok) return BROTLI_FALSE;
  return BROTLI_TRUE;
}
#endif  /* BROTLI_STATIC_INIT_NONE */

#if (BROTLI_STATIC_INIT == BROTLI_STATIC_INIT_EARLY)
static BROTLI_BOOL kEarlyInitOk;
static __attribute__((constructor)) void BrotliDecoderStaticInitEarly(void) {
  kEarlyInitOk = DoBrotliDecoderStaticInit();
}
#elif (BROTLI_STATIC_INIT == BROTLI_STATIC_INIT_LAZY)
static BROTLI_BOOL kLazyInitOk;
void BrotliDecoderLazyStaticInitInner(void) {
  kLazyInitOk = DoBrotliDecoderStaticInit();
}
#endif  /* BROTLI_STATIC_INIT_EARLY */

BROTLI_BOOL BrotliDecoderEnsureStaticInit(void) {
  BROTLI_BOOL ok = BROTLI_TRUE;
#if (BROTLI_STATIC_INIT == BROTLI_STATIC_INIT_EARLY)
  ok = kEarlyInitOk;
#elif (BROTLI_STATIC_INIT != BROTLI_STATIC_INIT_NONE)
  ok = kLazyInitOk;
#endif
  return ok;
}

#if defined(__cplusplus) || defined(c_plusplus)
}  /* extern "C" */
#endif
