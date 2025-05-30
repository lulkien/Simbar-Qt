#pragma once

#include <cstdint>
#include <qcolor.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

#include "theme.h"

#define DEFINE_CONST_PROPERTY(Type, name, value)                               \
  Q_PROPERTY(Type name READ name CONSTANT)                                     \
public:                                                                        \
  Type name() const { return value; }

#define DEFINE_THEME(QColor, color, defaultValue)                              \
  Q_PROPERTY(Type color READ color NOTIFY themeChanged)                        \
private:                                                                       \
  QColor m_##color = defaultValue;                                             \
                                                                               \
public:                                                                        \
  Type name() const { return m_##color; }

class Config final : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(SimbarConfig)
  QML_SINGLETON

  DEFINE_CONST_PROPERTY(int32_t, width, 3440)
  DEFINE_CONST_PROPERTY(int32_t, height, 45)

  DEFINE_CONST_PROPERTY(int32_t, qmlWidth, 3440)
  DEFINE_CONST_PROPERTY(int32_t, qmlHeight, 45)

  DEFINE_CONST_PROPERTY(int32_t, qmlDefaultWidgetSize, 35)

public:
  static Config& instance();
  static Config* create(QQmlEngine* /*unused*/, QJSEngine* /*unused*/);

signals:
  void themeChanged();

private:
  Config();
};

#define CONFIG Config::instance()
