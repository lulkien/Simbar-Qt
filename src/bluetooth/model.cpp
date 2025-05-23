#include "model.h"
#include <qobject.h>

BluetoothModel::BluetoothModel(QObject* parent) : QObject(parent) {}

BluetoothModel::~BluetoothModel() = default;

uint8_t BluetoothModel::setState(const uint8_t newState) {
  if (newState == m_state) {
    return m_state;
  }

  m_state = newState;
  emit stateChanged(m_state);

  return m_state;
}
