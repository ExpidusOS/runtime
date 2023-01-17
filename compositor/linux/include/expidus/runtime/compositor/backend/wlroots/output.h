#pragma once

#include <glib-object.h>
#include <wayland-server-core.h>
#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#include <expidus/runtime/compositor/output.h>
#include <wlr/types/wlr_output.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_OUTPUT expidus_runtime_compositor_wlroots_output_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorWlrootsOutput, expidus_runtime_compositor_wlroots_output, EXPIDUS_RUNTIME_COMPOSITOR, WLROOTS_OUTPUT, ExpidusRuntimeCompositorOutput);

struct _ExpidusRuntimeCompositorWlrootsOutput {
  ExpidusRuntimeCompositorOutput parent_instance;
  struct _ExpidusRuntimeCompositorWlrootsOutputPrivate* priv;
};

ExpidusRuntimeCompositorOutput* expidus_runtime_compositor_wlroots_output_new(ExpidusRuntimeCompositorWlrootsBackend* backend, struct wlr_output* value);

G_END_DECLS
