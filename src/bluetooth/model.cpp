#include "bluetooth/model.h"
#include <qobject.h>
#include <qqmlengine.h>

namespace Bluetooth {

Model::Model(QObject* parent) : QObject(parent) {
  QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Model::~Model() = default;

State Model::setState(const State newState) {
  if (newState == m_state) {
    return m_state;
  }

  m_state = newState;
  emit stateChanged(m_state);

  return m_state;
}

} // namespace Bluetooth
