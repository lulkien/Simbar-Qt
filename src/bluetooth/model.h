#pragma once

#include <memory>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include "src/bluetooth/common.h"

namespace Bluetooth {

class Model : public QObject {
  Q_OBJECT
  Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
  explicit Model(QObject* parent = nullptr);
  ~Model() override;

  [[nodiscard]] State state() const { return m_state; }
  State setState(State newState);

signals:
  void stateChanged(State state);

private:
  State m_state = State::Unknown;
};

} // namespace Bluetooth

using BluetoothModelRef = std::shared_ptr<Bluetooth::Model>;
