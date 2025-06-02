#pragma once

#include <LayerShellQt/window.h>
#include <qquickview.h>

#include "config.h"

struct SimbarView {
  LayerShellQt::Window::Anchor anchor;
  LayerShellQt::Window::Layer layer;
  int exclusiveZone;

  int width;
  int height;
  int x;
  int y;

  QQuickView* view = nullptr;
  LayerShellQt::Window* window = nullptr;

  bool autoWhenStart = false;
};

inline const QHash<QString, SimbarView> VIEW_MAP = {
    {"mainbar",
     {LayerShellQt::Window::AnchorTop, LayerShellQt::Window::LayerBottom,
      CONFIG.height(), CONFIG.width(), CONFIG.height(), 0, 0, nullptr, nullptr,
      true}}};
