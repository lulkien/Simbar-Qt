#pragma once

#include <memory.h>
#include <qobject.h>

#include "src/bluetooth/model.h"

namespace Bluetooth {

class Controller : public QObject {
public:
  Controller(QObject* parent = nullptr);
  ~Controller() override;

  [[nodiscard]] BluetoothModelRef getModel() const { return m_model; }

private:
  BluetoothModelRef m_model;
};

} // namespace Bluetooth

using BluetoothController = Bluetooth::Controller;
