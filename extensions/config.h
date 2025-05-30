#pragma once

#include <cstdint>
#include <qcolor.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

#include "theme.h"

#define DEFINE_CONST_PROPERTY(Type, name, value)                               \
  Q_PROPERTY(Type name READ name CONSTANT)                                     \
public:                                                                        \
  [[nodiscard]] Type name() const { return value; }

#define DEFINE_THEME(name)                                                     \
  Q_PROPERTY(QColor theme##name READ theme##name NOTIFY themeChanged)          \
private:                                                                       \
  QColor m_theme##name = Qt::white;                                            \
                                                                               \
public:                                                                        \
  [[nodiscard]] QColor theme##name() const { return m_theme##name; }

class Config final : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(SimbarConfig)
  QML_SINGLETON

  // Color config
  DEFINE_THEME(Rosewater)
  DEFINE_THEME(Flamingo)
  DEFINE_THEME(Pink)
  DEFINE_THEME(Mauve)
  DEFINE_THEME(Red)
  DEFINE_THEME(Maroon)
  DEFINE_THEME(Peach)
  DEFINE_THEME(Yellow)
  DEFINE_THEME(Green)
  DEFINE_THEME(Teal)
  DEFINE_THEME(Sky)
  DEFINE_THEME(Sapphire)
  DEFINE_THEME(Blue)
  DEFINE_THEME(Lavender)

  DEFINE_THEME(Text)
  DEFINE_THEME(Subtext1)
  DEFINE_THEME(Subtext0)
  DEFINE_THEME(Overlay2)
  DEFINE_THEME(Overlay1)
  DEFINE_THEME(Overlay0)
  DEFINE_THEME(Surface2)
  DEFINE_THEME(Surface1)
  DEFINE_THEME(Surface0)

  DEFINE_THEME(Base)
  DEFINE_THEME(Mantle)
  DEFINE_THEME(Crust)

  // Size config
  DEFINE_CONST_PROPERTY(int32_t, renderSample, 8)

  DEFINE_CONST_PROPERTY(int32_t, width, 3440)
  DEFINE_CONST_PROPERTY(int32_t, height, 45)

  DEFINE_CONST_PROPERTY(int32_t, qmlWidth, 3440)
  DEFINE_CONST_PROPERTY(int32_t, qmlHeight, 45)

  DEFINE_CONST_PROPERTY(int32_t, qmlDefaultBoxSize, 35)
  DEFINE_CONST_PROPERTY(int32_t, qmlDefaultPadding, 8)
  DEFINE_CONST_PROPERTY(int32_t, qmlDefaultFontsize, 16)

public:
  static Config& instance();
  static Config* create(QQmlEngine* /*unused*/, QJSEngine* /*unused*/);

  void loadTheme(const Theme& theme);

signals:
  void themeChanged();

private:
  Config();
};

#define CONFIG Config::instance()
