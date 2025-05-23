#include "src/bluetooth/controller.h"
#include "src/bluetooth/model.h"

#include <memory>
#include <qobject.h>

BluetoothController::BluetoothController(QObject* parent) : QObject{parent} {
  m_model = std::make_shared<BluetoothModel>();
}

BluetoothController::~BluetoothController() = default;
