#include <qguiapplication.h>
#include <qquickwindow.h>

#include "engine/engine.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQuickWindow::setGraphicsApi(QSGRendererInterface::Vulkan);

  ApplicationEngine engine;

  engine.initialize();
  engine.setupView();
  engine.showView();

  return QGuiApplication::exec();
}
