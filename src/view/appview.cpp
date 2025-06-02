#include "appview.h"
#include <memory>

ApplicationView::Builder::Builder()
    : m_name{"nonamed"}, m_layer{LayerShellQt::Window::LayerBottom},
      m_anchor{LayerShellQt::Window::AnchorTop}, m_exclusiveZone{0}, m_width{0},
      m_height{0}, m_posX{0}, m_posY{0}, m_created{false}, m_sample{4},
      m_autoShow{false} {}

ApplicationView::Builder&
ApplicationView::Builder::withName(const QString& name) {
  Q_ASSERT(!m_created);

  m_name = name;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withLayer(const LayerShellQt::Window::Layer& layer) {
  Q_ASSERT(!m_created);

  m_layer = layer;
  return *this;
}

ApplicationView::Builder& ApplicationView::Builder::withAnchor(
    const LayerShellQt::Window::Anchor& anchor) {
  Q_ASSERT(!m_created);

  m_anchor = anchor;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withExclusiveZone(const int32_t& zone) {
  Q_ASSERT(!m_created);

  m_exclusiveZone = zone;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withWidth(const int32_t& width) {
  Q_ASSERT(!m_created);

  m_width = width;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withHeight(const int32_t& height) {
  Q_ASSERT(!m_created);

  m_height = height;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withPositionX(const int32_t& posX) {
  Q_ASSERT(!m_created);

  m_posX = posX;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withPositionY(const int32_t& posY) {
  Q_ASSERT(!m_created);

  m_posY = posY;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withSample(const int32_t& sample) {
  Q_ASSERT(!m_created);

  m_sample = sample;
  return *this;
}

ApplicationView::Builder&
ApplicationView::Builder::withShowByDefault(const bool show) {
  Q_ASSERT(!m_created);

  m_autoShow = show;
  return *this;
}

ApplicationViewPtr ApplicationView::Builder::create() {
  Q_ASSERT(!m_created);

  m_created = true;

  ApplicationViewPtr exclusiveView(new ApplicationView);

  // Set auto show state
  exclusiveView->m_autoShow = m_autoShow;

  // Set format for supersampling
  QSurfaceFormat format = exclusiveView->m_view.format();
  format.setSamples(m_sample);
  exclusiveView->m_view.setFormat(format);

  // Set detail LayerShell
  exclusiveView->m_window = LayerShellQt::Window::get(&exclusiveView->m_view);

  exclusiveView->m_window->setLayer(m_layer);
  exclusiveView->m_window->setAnchors(m_anchor);

  if (0 != m_exclusiveZone) {
    exclusiveView->m_window->setExclusiveZone(m_exclusiveZone);
  }

  exclusiveView->m_window->setKeyboardInteractivity(
      LayerShellQt::Window::KeyboardInteractivityNone);

  // Set geometry for qquickview
  exclusiveView->m_view.setGeometry(m_posX, m_posY, m_width, m_height);

  return exclusiveView;
}

// #################################################################

QQuickView& ApplicationView::asView() {
  Q_ASSERT(m_window != nullptr);

  return m_view;
}
