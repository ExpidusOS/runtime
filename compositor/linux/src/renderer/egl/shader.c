#include <expidus/runtime/compositor/renderer/egl/error.h>
#include <expidus/runtime/compositor/renderer/egl/shader.h>
#include <gio/gio.h>
#include "error-priv.h"
#include "shader-priv.h"

static void g_initable_interface_init(GInitableIface* interface);

static const GLchar quad_vertex_src[] =
"attribute vec2 pos;\n"
"attribute vec2 texcoord;\n"
"varying vec2 v_texcoord;\n"
"\n"
"void main() {\n"
"	gl_Position = vec4(pos, 1.0, 1.0);\n"
"	v_texcoord = texcoord;\n"
"}\n";

static const GLchar tex_fragment_src_rgbx[] =
"precision mediump float;\n"
"varying vec2 v_texcoord;\n"
"uniform sampler2D tex;\n"
"\n"
"void main() {\n"
"  gl_FragColor = texture2D(tex, v_texcoord);\n"
"}\n";

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorEglShader, expidus_runtime_compositor_egl_shader, G_TYPE_OBJECT,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorEglShader)
    G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, g_initable_interface_init));

static GLuint expidus_runtime_compositor_egl_shader_compile_shader(GLuint type, const GLchar* src, GError** error) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint ok;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);

  if (ok == GL_FALSE) {
    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0) {
      GLchar* data = calloc(1, sizeof (GLchar) * log_length);
      g_assert(data != NULL);

      glGetShaderInfoLog(shader, log_length, &ok, data);
      expidus_runtime_compositor_egl_error_set_shader(error, data);
    } else {
      expidus_runtime_compositor_egl_error_set_shader(error, "unknown error");
    }

    glDeleteProgram(shader);
    return 0;
  }
  return shader;
}

static void expidus_runtime_compositor_egl_shader_finalize(GObject* object) {
  ExpidusRuntimeCompositorEglShader* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SHADER(object);
  g_assert(self != NULL);

  if (self->priv->prog > 0) {
    glDeleteProgram(self->priv->prog);
    self->priv->prog = 0;
  }

  G_OBJECT_CLASS(expidus_runtime_compositor_egl_shader_parent_class)->finalize(object);
}

static gboolean expidus_runtime_compositor_egl_shader_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  ExpidusRuntimeCompositorEglShader* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SHADER(initable);
  g_assert(self != NULL);

  GLuint vert = expidus_runtime_compositor_egl_shader_compile_shader(GL_VERTEX_SHADER, quad_vertex_src, error);
  if (vert == 0) return FALSE;

  GLuint frag = expidus_runtime_compositor_egl_shader_compile_shader(GL_FRAGMENT_SHADER, tex_fragment_src_rgbx, error);
  if (frag == 0) {
    glDeleteShader(vert);
    return FALSE;
  }

  self->priv->prog = glCreateProgram();
  glAttachShader(self->priv->prog, vert);
  glAttachShader(self->priv->prog, frag);
  glLinkProgram(self->priv->prog);

  glDetachShader(self->priv->prog, vert);
  glDetachShader(self->priv->prog, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);

  GLint ok;
  glGetProgramiv(self->priv->prog, GL_LINK_STATUS, &ok);
  if (ok == GL_FALSE) {
    glDeleteProgram(self->priv->prog);
    expidus_runtime_compositor_egl_error_set_shader(error, "failed to link");
    return FALSE;
  }

  self->priv->proj = glGetUniformLocation(self->priv->prog, "proj");
  self->priv->tex = glGetUniformLocation(self->priv->prog, "tex");
  self->priv->alpha = glGetUniformLocation(self->priv->prog, "alpha");
  self->priv->pos_attrib = glGetAttribLocation(self->priv->prog, "pos");
  self->priv->tex_attrib = glGetAttribLocation(self->priv->prog, "texcoord");
  return TRUE;
}

static void g_initable_interface_init(GInitableIface* interface) {
  interface->init = expidus_runtime_compositor_egl_shader_initable_init;
}

static void expidus_runtime_compositor_egl_shader_class_init(ExpidusRuntimeCompositorEglShaderClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = expidus_runtime_compositor_egl_shader_finalize;
}

static void expidus_runtime_compositor_egl_shader_init(ExpidusRuntimeCompositorEglShader* self) {
  ExpidusRuntimeCompositorEglShaderPrivate* priv = expidus_runtime_compositor_egl_shader_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
}

ExpidusRuntimeCompositorEglShader* expidus_runtime_compositor_egl_shader_new(GError** error) {
  return g_initable_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_SHADER, NULL, error, NULL);
}
