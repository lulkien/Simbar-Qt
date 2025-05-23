#pragma once

#include <cstdint>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlintegration.h>

class BluetoothEnums : public QObject {
  Q_OBJECT
public:
  enum class State : uint8_t {
    Unknown = 0,
  };
  Q_ENUM(State)
};
