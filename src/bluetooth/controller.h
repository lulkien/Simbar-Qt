#pragma once

#include <memory.h>
#include <qobject.h>

#include "src/bluetooth/model.h"

namespace Bluetooth {

class Controller : public QObject {
public:
  Controller(QObject* parent = nullptr);
  ~Controller() override;

  [[nodiscard]] ModelRef getModel() const { return m_model; }

private:
  ModelRef m_model;
};

} // namespace Bluetooth
