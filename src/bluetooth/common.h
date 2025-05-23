#pragma once

#include <cstdint>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

namespace Bluetooth {
Q_NAMESPACE
QML_ELEMENT
QML_NAMED_ELEMENT(BluetoothEnums)

enum class State : uint8_t {
  Unknown = 0,
  Unavailable,
  PoweredOff,
  PoweredOn,
  Discovering,
  Discoverable,
  Connected,
  Disconnected,
  Error
};
Q_ENUM_NS(State)

} // namespace Bluetooth
