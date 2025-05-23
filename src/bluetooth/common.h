#pragma once

#include <cstdint>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

namespace Bluetooth {
Q_NAMESPACE
QML_ELEMENT

enum class State : uint8_t {
  Unknown = 0,
  Idle,
};
Q_ENUM_NS(State)

} // namespace Bluetooth
