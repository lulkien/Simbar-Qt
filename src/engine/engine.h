#pragma once

#include <LayerShellQt/window.h>
#include <qquickview.h>

#include "appview.h"
#include "src/bluetooth/controller.h"

class ApplicationEngine {
public:
  ApplicationEngine();
  virtual ~ApplicationEngine();

  void initialize();
  void showView();

private:
  // Add and implement your functions here to create more views
  // Then put them in initialize()
  // ----------------------------------------------
  void createMainBar();
  // ----------------------------------------------

  BluetoothController m_btController;

  QHash<QString, ApplicationViewPtr> m_viewMap;
};
