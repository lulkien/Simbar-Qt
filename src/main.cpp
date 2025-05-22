#include <LayerShellQt/window.h>
#include <QGuiApplication>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QRasterWindow>
#include <QUrl>
#include <QtDebug>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  QQuickView window;
  LayerShellQt::Window* layerShell = LayerShellQt::Window::get(&window);

  // Create layershell for window
  layerShell->setLayer(LayerShellQt::Window::LayerBottom);
  layerShell->setAnchors(LayerShellQt::Window::AnchorTop);
  layerShell->setKeyboardInteractivity(
      LayerShellQt::Window::KeyboardInteractivityNone);
  layerShell->setExclusiveZone(40);

  // Load window content
  window.loadFromModule("SimbarQt", "Main");
  window.setGeometry(0, 0, 3440, 40);

  window.show();

  return QGuiApplication::exec();
}
