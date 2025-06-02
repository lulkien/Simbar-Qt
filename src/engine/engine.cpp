#include "engine.h"
#include "appview.h"
#include "config.h"
#include "mocha.h"
#include "theme.h"

#include <qdebug.h>
#include <qlogging.h>
#include <qqmlcontext.h>
#include <qquickview.h>
#include <qtmetamacros.h>

#include <LayerShellQt/window.h>

ApplicationEngine::ApplicationEngine() = default;

ApplicationEngine::~ApplicationEngine() = default;

void ApplicationEngine::initialize() {
  qDebug() << "Load theme: Mocha";
  CONFIG.loadTheme(CATPUCCIN_MOCHA);

  createMainBar();
}

void ApplicationEngine::showView() {
  for (const auto& viewName : m_viewMap.keys()) {
    const auto& appView = m_viewMap.value(viewName);
    if (appView->autoShow()) {
      appView->asView().show();
    }
  }
}

void ApplicationEngine::createMainBar() {
  qDebug() << "Setup: MainBar";
  auto mainView = ApplicationView::Builder()
                      .withName("MainBar")
                      .withSample(CONFIG.renderSample())
                      .withAnchor(LayerShellQt::Window::AnchorTop)
                      .withLayer(LayerShellQt::Window::LayerBottom)
                      .withExclusiveZone(CONFIG.height())
                      .withWidth(CONFIG.width())
                      .withHeight(CONFIG.height())
                      .withPositionX(0)
                      .withPositionY(0)
                      .withShowByDefault(true)
                      .create();

  qDebug() << "Set context properties for MainBar";
  mainView->asView().rootContext()->setContextProperty(
      "btModel", m_btController.getModel().get());

  mainView->asView().loadFromModule("Simbar", "Main");

  m_viewMap.insert(mainView->name(), mainView);
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
