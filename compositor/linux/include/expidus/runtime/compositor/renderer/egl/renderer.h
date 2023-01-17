#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/renderer.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_RENDERER expidus_runtime_compositor_egl_renderer_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorEglRenderer, expidus_runtime_compositor_egl_renderer, EXPIDUS_RUNTIME_COMPOSITOR, EGL_RENDERER, ExpidusRuntimeCompositorRenderer);

struct _ExpidusRuntimeCompositorEglRenderer {
  ExpidusRuntimeCompositorRenderer parent_instance;
  struct _ExpidusRuntimeCompositorEglRendererPrivate* priv;
};

ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_egl_renderer_new(EGLDisplay display, EGLContext context);

G_END_DECLS
