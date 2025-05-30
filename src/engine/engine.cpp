#include "engine.h"
#include "config.h"
#include "theme.h"

#include <exception>
#include <qdebug.h>
#include <qlogging.h>
#include <qqmlcontext.h>

#include <LayerShellQt/window.h>
#include <qtmetamacros.h>

#include "mocha.h"

ApplicationEngine::ApplicationEngine() = default;

ApplicationEngine::~ApplicationEngine() = default;

void ApplicationEngine::initialize() {
  qDebug() << "Load theme: Mocha";
  CONFIG.loadTheme(CATPUCCIN_MOCHA);

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
  format.setSamples(CONFIG.renderSample());
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

void Config::loadTheme(const Theme& theme) {
  m_themeRosewater = theme.rosewater;
  m_themeFlamingo = theme.flamingo;
  m_themePink = theme.pink;
  m_themeMauve = theme.mauve;
  m_themeRed = theme.red;
  m_themeMaroon = theme.maroon;
  m_themePeach = theme.peach;
  m_themeYellow = theme.yellow;
  m_themeGreen = theme.green;
  m_themeTeal = theme.teal;
  m_themeSky = theme.sky;
  m_themeSapphire = theme.sapphire;
  m_themeBlue = theme.blue;
  m_themeLavender = theme.lavender;

  m_themeText = theme.text;
  m_themeSubtext1 = theme.subtext1;
  m_themeSubtext0 = theme.subtext0;
  m_themeOverlay2 = theme.overlay2;
  m_themeOverlay1 = theme.overlay1;
  m_themeOverlay0 = theme.overlay0;
  m_themeSurface2 = theme.surface2;
  m_themeSurface1 = theme.surface1;
  m_themeSurface0 = theme.surface0;

  m_themeBase = theme.base;
  m_themeMantle = theme.mantle;
  m_themeCrust = theme.crust;

  emit themeChanged();
}
