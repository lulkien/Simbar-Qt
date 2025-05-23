#pragma once

#include <memory.h>
#include <qobject.h>

#include "src/bluetooth/model.h"

class BluetoothController : public QObject {
public:
  BluetoothController(QObject* parent = nullptr);
  ~BluetoothController() override;

  [[nodiscard]] BluetoothModelRef getModel() const { return m_model; }

private:
  BluetoothModelRef m_model;
};
