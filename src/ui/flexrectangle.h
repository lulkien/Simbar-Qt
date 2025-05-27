#pragma once

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <cstdint>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qquickrhiitem.h>
#include <qsggeometry.h>

namespace UI {

class FlexRectangle : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(
      QVariantList radius READ radius WRITE setRadius NOTIFY radiusChanged)
  Q_PROPERTY(
      uint32_t segments READ segments WRITE setSegments NOTIFY segmentsChanged)

public:
  FlexRectangle();

  [[nodiscard]] QColor color() const { return m_color; }
  void setColor(const QColor& color);

  [[nodiscard]] QVariantList radius() const { return m_radius; }
  void setRadius(const QVariantList& radius);

  [[nodiscard]] uint32_t segments() const { return m_segments; }
  void setSegments(uint32_t newSegments);

  QSGNode* updatePaintNode(QSGNode* oldNode,
                           UpdatePaintNodeData* data) override;

signals:
  void colorChanged();
  void radiusChanged();
  void segmentsChanged();

private:
  struct CornerRadii {
    float topLeft;
    float topRight;
    float bottomRight;
    float bottomLeft;

    void fromQVariantList(const QVariantList& list);
    void clampRadius(const float& width, const float& height);
  };

  [[nodiscard]] QSGGeometry* generateGeometry(const CornerRadii& radii) const;

  bool m_geometryDirty = true;
  uint32_t m_segments = 8;
  QColor m_color = Qt::white;
  QVariantList m_radius = {4, 4, 4, 4};
};

} // namespace UI
