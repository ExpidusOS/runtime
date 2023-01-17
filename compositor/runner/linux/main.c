#include <expidus/runtime/compositor.h>
#include <assert.h>
#include <stdlib.h>

#ifndef APPLICATION_ID
#error "Missing APPLICATION_ID"
#endif

#ifndef BINARY_NAME
#error "Missing BINARY_NAME"
#endif

int main(int argc, char** argv) {
  ExpidusRuntimeCompositor* comp = expidus_runtime_compositor_new(APPLICATION_ID, BINARY_NAME);
  assert(comp != NULL);
  return g_application_run(G_APPLICATION(comp), argc, argv);
}
