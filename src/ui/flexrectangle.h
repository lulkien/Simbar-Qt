#ifndef FLEXRECTANGLE_H
#define FLEXRECTANGLE_H

#include <QColor>
#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGNode>
#include <QVariantList>
#include <qqmlintegration.h>

class FlexRectangle : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(
      QVariantList radius READ radius WRITE setRadius NOTIFY radiusChanged)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY
                 borderColorChanged)
  Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY
                 borderWidthChanged)

public:
  explicit FlexRectangle(QQuickItem* parent = nullptr);

  [[nodiscard]] QColor color() const { return m_color; }
  void setColor(const QColor& color);

  [[nodiscard]] QVariantList radius() const { return m_radius; }
  void setRadius(const QVariantList& radius);

  [[nodiscard]] QColor borderColor() const { return m_borderColor; }
  void setBorderColor(const QColor& borderColor);

  [[nodiscard]] qreal borderWidth() const { return m_borderWidth; }
  void setBorderWidth(qreal borderWidth);

protected:
  QSGNode* updatePaintNode(QSGNode* oldNode,
                           UpdatePaintNodeData* nodeData) override;

signals:
  void colorChanged();
  void radiusChanged();
  void borderColorChanged();
  void borderWidthChanged();

private:
  struct CornerRadii {
    qreal topLeft;
    qreal topRight;
    qreal bottomRight;
    qreal bottomLeft;
  };

  void parseRadius();

  void createRoundedRectGeometry(QSGGeometry* geometry, const QRectF& rect,
                                 const CornerRadii& radii, int segments = 16);

  void createBorderGeometry(QSGGeometry* geometry, const QRectF& rect,
                            const CornerRadii& radii, qreal borderWidth,
                            int segments = 16);

  void addCornerVertices(QVector<QSGGeometry::Point2D>& vertices,
                         QVector<quint16>& indices, const QPointF& center,
                         qreal radius, qreal startAngle, qreal endAngle,
                         int segments, quint16& currentIndex,
                         bool isOuter = true);

  QColor m_color;
  QVariantList m_radius;
  QColor m_borderColor;
  qreal m_borderWidth;
  CornerRadii m_cornerRadii;
  bool m_geometryChanged;
};

#endif // FLEXRECTANGLE_H
