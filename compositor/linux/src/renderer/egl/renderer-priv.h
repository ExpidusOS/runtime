#pragma once

#include <expidus/runtime/compositor/renderer/egl/renderer.h>

typedef struct _ExpidusRuntimeCompositorEglRendererPrivate {
  FlutterRendererConfig config;
  EGLDisplay display;
  EGLContext context;
} ExpidusRuntimeCompositorEglRendererPrivate;
