#pragma once

#include "src/bluetooth/controller.h"
#include <LayerShellQt/window.h>
#include <qquickview.h>

class ApplicationEngine {
public:
  ApplicationEngine();
  virtual ~ApplicationEngine();

  void initialize();
  void setupView();
  void showView();

private:
  Bluetooth::Controller m_btController;

  QQuickView m_view;
  LayerShellQt::Window* m_layerShell = nullptr;
};
