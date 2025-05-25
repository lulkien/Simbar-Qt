#pragma once

#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qqmlintegration.h>

class FlexRectangleItem : public QQuickItem {
  Q_OBJECT
  QML_NAMED_ELEMENT(FlexRectangle)

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(
      QVariantList radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
  FlexRectangleItem(QQuickItem* parent = nullptr);

  [[nodiscard]] QColor color() const { return m_color; }
  void setColor(const QColor& color);

  [[nodiscard]] QVariantList radius() const { return m_radius; }
  void setRadius(const QVariantList& radius);

  QSGNode* updatePaintNode(QSGNode* oldNode,
                           UpdatePaintNodeData* data) override;

signals:
  void colorChanged();
  void radiusChanged();

private:
  struct CornerRadii {
    double topLeft;
    double topRight;
    double bottomRight;
    double bottomLeft;

    void fromQVariantList(const QVariantList& list);
    void clampRadius(const double& width, const double& height);
  };

  void generateGeometry(QSGGeometry* geometry, const CornerRadii& radii) const;

  int m_segments = 16;
  bool m_geometryDirty = true;
  QColor m_color = Qt::white;
  QVariantList m_radius = {4, 4, 4, 4};
};
