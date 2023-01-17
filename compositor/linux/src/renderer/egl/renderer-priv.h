#pragma once

#include <expidus/runtime/compositor/renderer/egl/renderer.h>

typedef struct _ExpidusRuntimeCompositorEglRendererPrivate {
  FlutterRendererConfig config;
  EGLDisplay display;
  EGLContext context;
  EGLContext flutter_context;
  EGLContext flutter_resource_context;
} ExpidusRuntimeCompositorEglRendererPrivate;
