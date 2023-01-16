#include <expidus/runtime/compositor.h>
#include <stdbool.h>

bool expidus_runtime_compositor_has_wlroots() {
#ifdef HAS_WLROOTS
  return true;
#else
  return false;
#endif
}
