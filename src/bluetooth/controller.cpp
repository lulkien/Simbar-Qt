#include "src/bluetooth/controller.h"
#include "src/bluetooth/model.h"

#include <memory>
#include <qobject.h>
#include <qqml.h>

namespace Bluetooth {

Controller::Controller(QObject* parent) : QObject{parent} {
  m_model = std::make_shared<Model>();
}

Controller::~Controller() = default;

} // namespace Bluetooth
