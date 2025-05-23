#pragma once

#include <memory>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include "src/bluetooth/common.h"

class BluetoothModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(uint8_t state READ state WRITE setState NOTIFY stateChanged)

public:
  explicit BluetoothModel(QObject* parent = nullptr);
  ~BluetoothModel() override;

  [[nodiscard]] uint8_t state() const { return m_state; }
  uint8_t setState(uint8_t newState);

signals:
  void stateChanged(uint8_t state);

private:
  uint8_t m_state = static_cast<uint8_t>(BluetoothEnums::State::Unknown);
};

using BluetoothModelRef = std::shared_ptr<BluetoothModel>;
