#pragma once

#include <expidus/runtime/compositor/renderer/egl/shader.h>
#include <GLES2/gl2.h>

typedef struct _ExpidusRuntimeCompositorEglShaderPrivate {
  GLuint prog;

  GLint proj;
  GLint tex;
  GLint alpha;
  GLint pos_attrib;
  GLint tex_attrib;
} ExpidusRuntimeCompositorEglShaderPrivate;
