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

  m_layerShell->setExclusiveZone(CONFIG.height());

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
  m_view.setGeometry((CONFIG.width() - CONFIG.qmlWidth()) / 2,
                     (CONFIG.height() - CONFIG.qmlHeight()) / 2, CONFIG.width(),
                     CONFIG.height());
}

void ApplicationEngine::showView() {

  if (nullptr == m_layerShell) {
    throw std::exception();
  }

  m_view.show();
}

// ####################### Config implementation #######################

Config::Config() {
  QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Config& Config::instance() {
  static Config self;
  return self;
}

Config* Config::create(QQmlEngine* /*unused*/, QJSEngine* /*unused*/) {
  return &instance();
}
