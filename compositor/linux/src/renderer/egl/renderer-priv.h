#pragma once

#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include <expidus/runtime/compositor/renderer/egl/shader.h>
#include <expidus/runtime/compositor/rendering/scene.h>
#include <GLES2/gl2.h>

struct PageTexture {
  struct Page* page;

  GLuint texture;
  GLuint fbo;

  size_t width;
  size_t height;
};

struct Page {
  GList* unused_textures;
  GList* textures;
};

typedef struct _ExpidusRuntimeCompositorEglRendererPrivate {
  FlutterRendererConfig config;
  FlutterCompositor compositor;

  ExpidusRuntimeCompositorEglShader* shader;
  ExpidusRuntimeCompositorScene* scene;
  struct Page pages[2];
  uint8_t current_page;

  EGLDisplay display;

  EGLContext context;
  EGLContext flutter_context;
  EGLContext flutter_resource_context;

  EGLSync sync;

  GLuint tex_coord_buffer;
  GLuint quad_vert_buffer;
} ExpidusRuntimeCompositorEglRendererPrivate;
