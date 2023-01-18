#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_SHADER expidus_runtime_compositor_egl_shader_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorEglShader, expidus_runtime_compositor_egl_shader, EXPIDUS_RUNTIME_COMPOSITOR, EGL_SHADER, GObject);

struct _ExpidusRuntimeCompositorEglShader {
  GObject parent_instance;
  struct _ExpidusRuntimeCompositorEglShaderPrivate* priv;
};

ExpidusRuntimeCompositorEglShader* expidus_runtime_compositor_egl_shader_new(GError** error);

G_END_DECLS
