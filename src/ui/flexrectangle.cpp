#include "flexrectangle.h"

#include <QDebug>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QtMath>

FlexRectangle::FlexRectangle(QQuickItem* parent)
    : QQuickItem(parent), m_color(Qt::transparent), m_borderColor(Qt::black),
      m_borderWidth(0.0), m_geometryChanged(true) {

  setFlag(ItemHasContents, true);
  m_radius = QVariantList{0.0};
  parseRadius();
}

void FlexRectangle::setColor(const QColor& color) {
  if (m_color == color) {
    return;
  }

  m_color = color;
  emit colorChanged();
  update();
}

void FlexRectangle::setRadius(const QVariantList& radius) {
  if (m_radius == radius) {
    return;
  }

  m_radius = radius;
  parseRadius();
  m_geometryChanged = true;
  emit radiusChanged();
  update();
}

void FlexRectangle::setBorderColor(const QColor& borderColor) {
  if (m_borderColor == borderColor) {
    return;
  }

  m_borderColor = borderColor;
  emit borderColorChanged();
  update();
}

void FlexRectangle::setBorderWidth(qreal borderWidth) {
  borderWidth = qMax(0.0, borderWidth);

  if (qFuzzyCompare(m_borderWidth, borderWidth)) {
    return;
  }

  m_borderWidth = borderWidth;
  m_geometryChanged = true;
  emit borderWidthChanged();
  update();
}

void FlexRectangle::parseRadius() {
  if (m_radius.isEmpty()) {
    m_cornerRadii = {0.0, 0.0, 0.0, 0.0};
    return;
  }

  auto getValue = [](const QVariant& value) -> qreal {
    bool is_ok;
    qreal val = value.toReal(&is_ok);
    return is_ok ? qMax(0.0, val) : 0.0;
  };

  if (m_radius.size() == 1) {
    qreal radius = getValue(m_radius[0]);
    m_cornerRadii = {radius, radius, radius, radius};
  } else if (m_radius.size() >= 4) {
    m_cornerRadii.topLeft = getValue(m_radius[0]);
    m_cornerRadii.topRight = getValue(m_radius[1]);
    m_cornerRadii.bottomRight = getValue(m_radius[2]);
    m_cornerRadii.bottomLeft = getValue(m_radius[3]);
  } else {
    qreal radius = getValue(m_radius[0]);
    m_cornerRadii = {radius, radius, radius, radius};
  }
}

QSGNode* FlexRectangle::updatePaintNode(QSGNode* oldNode,
                                        UpdatePaintNodeData* /*unused*/) {
  QSGNode* rootNode = oldNode;

  if (nullptr == rootNode) {
    rootNode = new QSGNode;
  }

  // Clear existing children
  rootNode->removeAllChildNodes();

  QRectF rect(0, 0, width(), height());
  if (rect.width() <= 0 || rect.height() <= 0) {
    return rootNode;
  }

  qreal maxRadius = qMin(rect.width(), rect.height()) * 0.5;
  CornerRadii clampedRadii = {qMin(m_cornerRadii.topLeft, maxRadius),
                              qMin(m_cornerRadii.topRight, maxRadius),
                              qMin(m_cornerRadii.bottomRight, maxRadius),
                              qMin(m_cornerRadii.bottomLeft, maxRadius)};

  if (m_color.alpha() > 0) {
    auto* fillNode = new QSGGeometryNode;

    auto* fillGeometry =
        new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
    fillGeometry->setDrawingMode(QSGGeometry::DrawTriangles);
    createRoundedRectGeometry(fillGeometry, rect, clampedRadii);

    fillNode->setGeometry(fillGeometry);
    fillNode->setFlag(QSGNode::OwnsGeometry);

    auto* fillMaterial = new QSGFlatColorMaterial;
    fillMaterial->setColor(m_color);
    fillNode->setMaterial(fillMaterial);
    fillNode->setFlag(QSGNode::OwnsMaterial);

    rootNode->appendChildNode(fillNode);
  }

  // transparent
  if (m_borderWidth > 0 && m_borderColor.alpha() > 0) {
    auto* borderNode = new QSGGeometryNode;

    auto* borderGeometry =
        new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
    borderGeometry->setDrawingMode(QSGGeometry::DrawTriangles);
    createBorderGeometry(borderGeometry, rect, clampedRadii, m_borderWidth);

    borderNode->setGeometry(borderGeometry);
    borderNode->setFlag(QSGNode::OwnsGeometry);

    auto* borderMaterial = new QSGFlatColorMaterial;
    borderMaterial->setColor(m_borderColor);
    borderNode->setMaterial(borderMaterial);
    borderNode->setFlag(QSGNode::OwnsMaterial);

    rootNode->appendChildNode(borderNode);
  }

  m_geometryChanged = false;
  return rootNode;
}

void FlexRectangle::createRoundedRectGeometry(QSGGeometry* geometry,
                                              const QRectF& rect,
                                              const CornerRadii& radii,
                                              int segments) {
  QVector<QSGGeometry::Point2D> vertices;
  QVector<quint16> indices;

  // Center point for triangle fan approach
  QPointF center = rect.center();
  vertices.append(
      {static_cast<float>(center.x()), static_cast<float>(center.y())});

  quint16 currentIndex = 1;

  // Helper lambda to add straight edge vertices
  auto addEdgeVertex = [&](const QPointF& point) {
    vertices.append(
        {static_cast<float>(point.x()), static_cast<float>(point.y())});
    if (currentIndex > 1) {
      indices.append(0);
      indices.append(currentIndex - 1);
      indices.append(currentIndex);
    }
    currentIndex++;
  };

  // Top edge (right to left, accounting for corner radii)
  if (radii.topRight > 0) {
    addCornerVertices(
        vertices, indices,
        QPointF(rect.right() - radii.topRight, rect.top() + radii.topRight),
        radii.topRight, -M_PI_2, 0, segments / 4, currentIndex, true);
  }
  addEdgeVertex(QPointF(rect.left() + radii.topLeft, rect.top()));

  // Top-left corner
  if (radii.topLeft > 0) {
    addCornerVertices(
        vertices, indices,
        QPointF(rect.left() + radii.topLeft, rect.top() + radii.topLeft),
        radii.topLeft, M_PI, M_PI_2, segments / 4, currentIndex, true);
  }

  // Left edge
  addEdgeVertex(QPointF(rect.left(), rect.bottom() - radii.bottomLeft));

  // Bottom-left corner
  if (radii.bottomLeft > 0) {
    addCornerVertices(vertices, indices,
                      QPointF(rect.left() + radii.bottomLeft,
                              rect.bottom() - radii.bottomLeft),
                      radii.bottomLeft, M_PI_2, M_PI, segments / 4,
                      currentIndex, true);
  }

  // Bottom edge
  addEdgeVertex(QPointF(rect.right() - radii.bottomRight, rect.bottom()));

  // Bottom-right corner
  if (radii.bottomRight > 0) {
    addCornerVertices(vertices, indices,
                      QPointF(rect.right() - radii.bottomRight,
                              rect.bottom() - radii.bottomRight),
                      radii.bottomRight, 0, M_PI_2, segments / 4, currentIndex,
                      true);
  }

  // Right edge - close the shape
  addEdgeVertex(QPointF(rect.right(), rect.top() + radii.topRight));

  // Close the shape
  if (currentIndex > 2) {
    indices.append(0);
    indices.append(currentIndex - 1);
    indices.append(1);
  }

  // Allocate and copy data
  geometry->allocate(vertices.size(), indices.size());
  memcpy(geometry->vertexData(), vertices.constData(),
         vertices.size() * sizeof(QSGGeometry::Point2D));
  memcpy(geometry->indexData(), indices.constData(),
         indices.size() * sizeof(quint16));
}

void FlexRectangle::createBorderGeometry(QSGGeometry* geometry,
                                         const QRectF& rect,
                                         const CornerRadii& radii,
                                         qreal borderWidth, int segments) {
  QVector<QSGGeometry::Point2D> vertices;
  QVector<quint16> indices;

  // Create outer and inner rounded rectangles, then triangulate between them
  QRectF outerRect = rect;
  QRectF innerRect =
      rect.adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);

  CornerRadii innerRadii = {qMax(0.0, radii.topLeft - borderWidth),
                            qMax(0.0, radii.topRight - borderWidth),
                            qMax(0.0, radii.bottomRight - borderWidth),
                            qMax(0.0, radii.bottomLeft - borderWidth)};

  quint16 currentIndex = 0;

  // Generate vertices for each corner and edge
  struct EdgePoint {
    QPointF outer, inner;
  };

  QVector<EdgePoint> edgePoints;

  // Ensure segments/4 is valid. If segments is 0, loops won't run for curves.
  const int pointsPerCorner = (segments > 0) ? (segments / 4) : 0;

  // 1. Top-Left Corner
  if (radii.topLeft > 0 && pointsPerCorner > 0) {
    QPointF outerCenter(outerRect.left() + radii.topLeft,
                        outerRect.top() + radii.topLeft);
    QPointF innerCenter(innerRect.left() + innerRadii.topLeft,
                        innerRect.top() + innerRadii.topLeft);
    for (int i = 0; i <= pointsPerCorner; ++i) {
      qreal angle = (3 * M_PI_2) -
                    ((M_PI_2 * i) /
                     pointsPerCorner); // Angle from 270 deg down to 180 deg
      QPointF outerPt = outerCenter + QPointF(radii.topLeft * qCos(angle),
                                              radii.topLeft * qSin(angle));
      QPointF innerPt = innerCenter + QPointF(innerRadii.topLeft * qCos(angle),
                                              innerRadii.topLeft * qSin(angle));
      edgePoints.append({outerPt, innerPt});
    }
  } else {
    edgePoints.append({{outerRect.left(), outerRect.top()},
                       {innerRect.left(), innerRect.top()}});
  }

  // 2. Top-Right Corner
  if (radii.topRight > 0 && pointsPerCorner > 0) {
    QPointF outerCenter(outerRect.right() - radii.topRight,
                        outerRect.top() + radii.topRight);
    QPointF innerCenter(innerRect.right() - innerRadii.topRight,
                        innerRect.top() + innerRadii.topRight);
    for (int i = 0; i <= pointsPerCorner; ++i) {
      qreal angle =
          -M_PI_2 + ((M_PI_2 * i) /
                     pointsPerCorner); // Angle from -90/270 deg up to 0 deg
      QPointF outerPt = outerCenter + QPointF(radii.topRight * qCos(angle),
                                              radii.topRight * qSin(angle));
      QPointF innerPt =
          innerCenter + QPointF(innerRadii.topRight * qCos(angle),
                                innerRadii.topRight * qSin(angle));
      if (i == 0 && !edgePoints.isEmpty() &&
          edgePoints.last().outer == outerPt) {
        continue;
      }
      edgePoints.append({outerPt, innerPt});
    }
  } else {
    if (edgePoints.isEmpty() ||
        edgePoints.last().outer !=
            QPointF(outerRect.right(), outerRect.top())) {
      edgePoints.append({{outerRect.right(), outerRect.top()},
                         {innerRect.right(), innerRect.top()}});
    }
  }

  // 3. Bottom-Right Corner
  if (radii.bottomRight > 0 && pointsPerCorner > 0) {
    QPointF outerCenter(outerRect.right() - radii.bottomRight,
                        outerRect.bottom() - radii.bottomRight);
    QPointF innerCenter(innerRect.right() - innerRadii.bottomRight,
                        innerRect.bottom() - innerRadii.bottomRight);
    for (int i = 0; i <= pointsPerCorner; ++i) {
      qreal angle = 0 + ((M_PI_2 * i) /
                         pointsPerCorner); // Angle from 0 deg down to 90 deg
      QPointF outerPt = outerCenter + QPointF(radii.bottomRight * qCos(angle),
                                              radii.bottomRight * qSin(angle));
      QPointF innerPt =
          innerCenter + QPointF(innerRadii.bottomRight * qCos(angle),
                                innerRadii.bottomRight * qSin(angle));
      if (i == 0 && !edgePoints.isEmpty() &&
          edgePoints.last().outer == outerPt) {
        continue;
      }
      edgePoints.append({outerPt, innerPt});
    }
  } else {
    if (edgePoints.isEmpty() ||
        edgePoints.last().outer !=
            QPointF(outerRect.right(), outerRect.bottom())) {
      edgePoints.append({{outerRect.right(), outerRect.bottom()},
                         {innerRect.right(), innerRect.bottom()}});
    }
  }

  // 4. Bottom-Left Corner
  if (radii.bottomLeft > 0 && pointsPerCorner > 0) {
    QPointF outerCenter(outerRect.left() + radii.bottomLeft,
                        outerRect.bottom() - radii.bottomLeft);
    QPointF innerCenter(innerRect.left() + innerRadii.bottomLeft,
                        innerRect.bottom() - innerRadii.bottomLeft);
    for (int i = 0; i <= pointsPerCorner; ++i) {
      qreal angle =
          M_PI_2 +
          ((M_PI_2 * i) / pointsPerCorner); // Angle from 90 deg down to 180 deg
      QPointF outerPt = outerCenter + QPointF(radii.bottomLeft * qCos(angle),
                                              radii.bottomLeft * qSin(angle));
      QPointF innerPt =
          innerCenter + QPointF(innerRadii.bottomLeft * qCos(angle),
                                innerRadii.bottomLeft * qSin(angle));
      if (i == 0 && !edgePoints.isEmpty() &&
          edgePoints.last().outer == outerPt) {
        continue;
      }
      edgePoints.append({outerPt, innerPt});
    }
  } else {
    if (edgePoints.isEmpty() ||
        edgePoints.last().outer !=
            QPointF(outerRect.left(), outerRect.bottom())) {
      edgePoints.append({{outerRect.left(), outerRect.bottom()},
                         {innerRect.left(), innerRect.bottom()}});
    }
  }

  if (edgePoints.size() > 1 &&
      edgePoints.first().outer == edgePoints.last().outer) {
    edgePoints.removeLast();
  }

  // Convert edge points to triangle strip
  for (int i = 0; i < edgePoints.size(); ++i) {
    vertices.append({static_cast<float>(edgePoints[i].outer.x()),
                     static_cast<float>(edgePoints[i].outer.y())});
    vertices.append({static_cast<float>(edgePoints[i].inner.x()),
                     static_cast<float>(edgePoints[i].inner.y())});

    if (i > 0) {
      // Create two triangles for each quad
      quint16 base = (i - 1) * 2;
      indices.append(base);     // prev outer
      indices.append(base + 1); // prev inner
      indices.append(base + 2); // curr outer

      indices.append(base + 1); // prev inner
      indices.append(base + 3); // curr inner
      indices.append(base + 2); // curr outer
    }
  }

  geometry->allocate(vertices.size(), indices.size());
  if (vertices.size() > 0) {
    memcpy(geometry->vertexData(), vertices.constData(),
           vertices.size() * sizeof(QSGGeometry::Point2D));
  }
  if (indices.size() > 0) {
    memcpy(geometry->indexData(), indices.constData(),
           indices.size() * sizeof(quint16));
  }
}

void FlexRectangle::addCornerVertices(QVector<QSGGeometry::Point2D>& vertices,
                                      QVector<quint16>& indices,
                                      const QPointF& center, qreal radius,
                                      qreal startAngle, qreal endAngle,
                                      int segments, quint16& currentIndex,
                                      bool isOuter) {
  for (int i = 0; i <= segments; ++i) {
    qreal angle = startAngle + ((endAngle - startAngle) * i / segments);
    QPointF point =
        center + QPointF(radius * qCos(angle), radius * qSin(angle));
    vertices.append(
        {static_cast<float>(point.x()), static_cast<float>(point.y())});

    if (currentIndex > 1) {
      indices.append(0);
      indices.append(currentIndex - 1);
      indices.append(currentIndex);
    }
    currentIndex++;
  }
}
