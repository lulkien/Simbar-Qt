#include "flexrectangle.h"

#include <QSGGeometry>
#include <QSGVertexColorMaterial>
#include <cmath>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qminmax.h>
#include <qsggeometry.h>
#include <qtpreprocessorsupport.h>
#include <qvariant.h>

void FlexRectangleItem::CornerRadii::fromQVariantList(
    const QVariantList& list) {

  topLeft = 0.0F;
  topRight = 0.0F;
  bottomRight = 0.0F;
  bottomLeft = 0.0F;

  if (list.isEmpty()) {
    return;
  }

  auto toDouble = [](const QVariant& value) -> double {
    bool ok = false;
    double ret = value.toDouble(&ok);
    return ok ? ret : 0.0F;
  };

  switch (list.size()) {
  case 1:
    topLeft = toDouble(list.at(0));
    break;
  case 2:
    topLeft = toDouble(list.at(0));
    topRight = toDouble(list.at(1));
    break;
  case 3:
    topLeft = toDouble(list.at(0));
    topRight = toDouble(list.at(1));
    bottomRight = toDouble(list.at(2));
    break;
  default:
    topLeft = toDouble(list.at(0));
    topRight = toDouble(list.at(1));
    bottomRight = toDouble(list.at(2));
    bottomLeft = toDouble(list.at(3));
    break;
  }
}

void FlexRectangleItem::CornerRadii::clampRadius(const double& width,
                                                 const double& height) {
  double maxRadius = qMin(width / 2, height / 2);

  topLeft = qMax(0.0F, qMin(maxRadius, topLeft));
  topRight = qMax(0.0F, qMin(maxRadius, topRight));
  bottomRight = qMax(0.0F, qMin(maxRadius, bottomRight));
  bottomLeft = qMax(0.0F, qMin(maxRadius, bottomLeft));
}

// ###################################################################################

FlexRectangleItem::FlexRectangleItem(QQuickItem* parent) : QQuickItem(parent) {
  setFlag(ItemHasContents, true);
}

void FlexRectangleItem::setColor(const QColor& color) {
  if (m_color == color) {
    return;
  }
  m_color = color;
  emit colorChanged();
  update();
}

void FlexRectangleItem::setRadius(const QVariantList& radius) {
  if (m_radius == radius) {
    return;
  }
  m_radius = radius;
  while (m_radius.size() < 4) {
    m_radius.append(0);
  }
  m_geometryDirty = true;
  emit radiusChanged();
  update();
}

QSGNode* FlexRectangleItem::updatePaintNode(QSGNode* oldNode,
                                            UpdatePaintNodeData* data) {
  Q_UNUSED(data)

  const double width = this->width();
  const double height = this->height();

  if (width <= 0 || height <= 0) {
    delete oldNode;
    return nullptr;
  }

  auto* node = static_cast<QSGGeometryNode*>(oldNode);
  if ((node == nullptr) || m_geometryDirty) {
    delete node;
    node = new QSGGeometryNode;

    CornerRadii radii;
    radii.fromQVariantList(m_radius);
    radii.clampRadius(width, height);

    const int vertexCount = 5 * m_segments;

    auto* geometry =
        new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);

    geometry->setDrawingMode(QSGGeometry::DrawTriangles);

    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    generateGeometry(geometry, radii);

    auto* material = new QSGFlatColorMaterial;
    material->setColor(m_color);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    m_geometryDirty = false;
  } else {
    auto* material = static_cast<QSGFlatColorMaterial*>(node->material());
    if (material->color() != m_color) {
      material->setColor(m_color);
      node->markDirty(QSGNode::DirtyMaterial);
    }
  }

  return node;
}

void FlexRectangleItem::generateGeometry(QSGGeometry* geometry,
                                         const CornerRadii& radii) const {
  // TODO: implement this
  auto* vertices = geometry->vertexDataAsPoint2D();

  size_t vertex = 0;
  size_t repeat = 2;

  vertices[vertex].set(width() / 2, height() / 2);

  // Calculate TopLeft vertices
  for (int i = 0; i <= 16; i++) {
    double angle = i * (M_PI_2 / static_cast<double>(m_segments));

    double x = radii.topLeft * (1 - cos(angle));
    double y = radii.topLeft * (1 - sin(angle));

    vertices[++vertex].set(x, y);

    if (--repeat == 0) {
      repeat = 1;
      vertices[++vertex].set(x, y);
      vertices[++vertex].set(width() / 2, height() / 2);
    }
  }

  // Calculate TopRight vertices
  for (int i = 0; i <= 16; i++) {
    // double angle = i * (M_PI_2 / static_cast<double>(m_segments));
    //
    // double x = width() - radii.topRight * (1 - sin(angle));
    // double y = radii.topRight * (1 - cos(angle));
    //
    // vertices[++vertexIndex].set(x, y);
  }

  // Calculate BottomRight vertices
  for (int i = 0; i <= 16; i++) {
  }

  // Calculate BottomLeft vertices
  for (int i = 0; i <= 16; i++) {
  }

  qDebug() << vertex;
  /*
    code
  */

  // Last vertice to close loop
  // vertices[vertexIndex].set(vertices[1].x, vertices[1].y);
}
