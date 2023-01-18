#include <expidus/runtime/compositor/renderer.h>
#include <glib.h>

G_DEFINE_TYPE(ExpidusRuntimeCompositorRenderer, expidus_runtime_compositor_renderer, G_TYPE_OBJECT);

static void expidus_runtime_compositor_renderer_class_init(ExpidusRuntimeCompositorRendererClass* klass) {}

static void expidus_runtime_compositor_renderer_init(ExpidusRuntimeCompositorRenderer* self) {}

FlutterRendererConfig* expidus_runtime_compositor_renderer_get_config(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_config != NULL, NULL);
  return klass->get_config(self);
}

FlutterCompositor* expidus_runtime_compositor_renderer_get_compositor(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  if (klass->get_compositor != NULL) return klass->get_compositor(self);
  return NULL;
}

void expidus_runtime_compositor_renderer_wait_sync(ExpidusRuntimeCompositorRenderer* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self));

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  if (klass->wait_sync != NULL) klass->wait_sync(self);
}

ExpidusRuntimeCompositorSceneLayer* expidus_runtime_compositor_renderer_create_scene_layer(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->create_scene_layer != NULL, NULL);
  return klass->create_scene_layer(self);
}
