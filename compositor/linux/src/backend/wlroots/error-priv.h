#pragma once

#include <expidus/runtime/compositor/backend/wlroots/error.h>

void expidus_runtime_compositor_wlroots_error_set_drm(GError** error);
void expidus_runtime_compositor_wlroots_error_set_wayland(GError** error, const gchar* msg);
void expidus_runtime_compositor_wlroots_error_set_renderer(GError** error, const gchar* msg);
