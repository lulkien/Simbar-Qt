#include "engine.h"
#include "config.h"

#include <exception>
#include <qdebug.h>
#include <qlogging.h>
#include <qqmlcontext.h>

#include <LayerShellQt/window.h>

static const int MSAA_X8 = 8;

ApplicationEngine::ApplicationEngine() = default;

ApplicationEngine::~ApplicationEngine() = default;

void ApplicationEngine::initialize() {
  qDebug() << "Initialize layer shell";

  m_layerShell = LayerShellQt::Window::get(&m_view);

  // Create layershell for window
  m_layerShell->setLayer(LayerShellQt::Window::LayerBottom);
  m_layerShell->setAnchors(LayerShellQt::Window::AnchorTop);
  m_layerShell->setKeyboardInteractivity(
      LayerShellQt::Window::KeyboardInteractivityNone);
  m_layerShell->setExclusiveZone(BAR_HEIGHT);

  qDebug() << "Expose C++ QObject to QML";
  m_view.rootContext()->setContextProperty("btModel",
                                           m_btController.getModel().get());

  QSurfaceFormat format = m_view.format();
  format.setSamples(MSAA_X8);
  m_view.setFormat(format);
}

void ApplicationEngine::setupView() {
  qDebug() << "Setup view from module";
  m_view.loadFromModule("Simbar", "Main");
  m_view.setGeometry(BAR_X_POSITION, BAR_Y_POSITION, BAR_WIDTH, BAR_HEIGHT);
}

void ApplicationEngine::showView() {

  if (nullptr == m_layerShell) {
    throw std::exception();
  }

  m_view.show();
}
