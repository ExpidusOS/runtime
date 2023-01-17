#pragma once

#include <expidus/runtime/compositor/backend/xcb/backend.h>

#include <xcb/xcb.h>

typedef struct _ExpidusRuntimeCompositorXcbBackendPrivate {
  xcb_connection_t* connection;
} ExpidusRuntimeCompositorXcbBackendPrivate;
