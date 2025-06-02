#pragma once

#include <LayerShellQt/window.h>
#include <memory>
#include <qquickview.h>
#include <qtclasshelpermacros.h>

class ApplicationView {
public:
  class Builder {
    Q_DISABLE_COPY(Builder)

  public:
    Builder();

    Builder& withName(const QString& name);
    Builder& withLayer(const LayerShellQt::Window::Layer& layer);
    Builder& withAnchor(const LayerShellQt::Window::Anchor& anchor);
    Builder& withExclusiveZone(const int32_t& zone);
    Builder& withWidth(const int32_t& width);
    Builder& withHeight(const int32_t& height);
    Builder& withPositionX(const int32_t& posX);
    Builder& withPositionY(const int32_t& posY);
    Builder& withSample(const int32_t& sample);
    Builder& withShowByDefault(bool show);

    std::shared_ptr<ApplicationView> create();

  private:
    QString m_name;
    LayerShellQt::Window::Layer m_layer;
    LayerShellQt::Window::Anchor m_anchor;
    int32_t m_exclusiveZone;
    int32_t m_width;
    int32_t m_height;
    int32_t m_posX;
    int32_t m_posY;
    int32_t m_sample;

    bool m_autoShow;

    bool m_created;
  };

  QQuickView& asView();
  [[nodiscard]] const QString& name() const { return m_name; }
  [[nodiscard]] bool autoShow() const { return m_autoShow; }

private:
  ApplicationView() = default;

  QQuickView m_view;
  LayerShellQt::Window* m_window = nullptr;

  QString m_name;
  bool m_autoShow;
  friend class Builder;
};

using ApplicationViewPtr = std::shared_ptr<ApplicationView>;
