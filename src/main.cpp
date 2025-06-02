#include <qguiapplication.h>
#include <qquickwindow.h>

#include "engine/engine.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQuickWindow::setGraphicsApi(QSGRendererInterface::VulkanRhi);

  ApplicationEngine engine;

  engine.initialize();
  engine.showView();

  return QGuiApplication::exec();
}
