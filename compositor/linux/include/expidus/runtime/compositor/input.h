#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/backend.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_INPUT expidus_runtime_compositor_input_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorInput, expidus_runtime_compositor_input, EXPIDUS_RUNTIME_COMPOSITOR, INPUT, GObject);

struct _ExpidusRuntimeCompositorInputClass {
  GObjectClass parent_class;

  const gchar* (*get_name)(ExpidusRuntimeCompositorInput* self);
};

const gchar* expidus_runtime_compositor_input_get_name(ExpidusRuntimeCompositorInput* self);
ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_input_get_backend(ExpidusRuntimeCompositorInput* self);

G_END_DECLS
