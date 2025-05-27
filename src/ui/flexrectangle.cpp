#include "flexrectangle.h"

#include <QSGGeometry>
#include <QSGVertexColorMaterial>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qminmax.h>
#include <qsggeometry.h>
#include <qtpreprocessorsupport.h>
#include <qvariant.h>
#include <vector>

namespace UI {

void FlexRectangle::CornerRadii::fromQVariantList(const QVariantList& list) {

  topLeft = 0.0F;
  topRight = 0.0F;
  bottomRight = 0.0F;
  bottomLeft = 0.0F;

  if (list.isEmpty()) {
    return;
  }

  auto toFloat = [](const QVariant& value) -> float {
    bool ok = false;
    float ret = value.toFloat(&ok);
    return ok ? ret : 0.0F;
  };

  switch (list.size()) {
  case 1:
    topLeft = toFloat(list.at(0));
    break;
  case 2:
    topLeft = toFloat(list.at(0));
    topRight = toFloat(list.at(1));
    break;
  case 3:
    topLeft = toFloat(list.at(0));
    topRight = toFloat(list.at(1));
    bottomRight = toFloat(list.at(2));
    break;
  default:
    topLeft = toFloat(list.at(0));
    topRight = toFloat(list.at(1));
    bottomRight = toFloat(list.at(2));
    bottomLeft = toFloat(list.at(3));
    break;
  }
}

void FlexRectangle::CornerRadii::clampRadius(const float& width,
                                             const float& height) {
  float maxRadius = qMin(width / 2, height / 2);

  topLeft = qMax(0.0F, qMin(maxRadius, topLeft));
  topRight = qMax(0.0F, qMin(maxRadius, topRight));
  bottomRight = qMax(0.0F, qMin(maxRadius, bottomRight));
  bottomLeft = qMax(0.0F, qMin(maxRadius, bottomLeft));
}

// ###################################################################################

FlexRectangle::FlexRectangle() { this->setFlag(ItemHasContents, true); }

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

  while (m_radius.size() < 4) {
    m_radius.append(0);
  }

  m_geometryDirty = true;
  emit radiusChanged();
  update();
}

void FlexRectangle::setSegments(uint32_t newSegments) {
  if (m_segments == newSegments) {
    return;
  }
  m_segments = newSegments;
  m_geometryDirty = true;
  emit segmentsChanged();
  update();
}

QSGNode* FlexRectangle::updatePaintNode(QSGNode* oldNode,
                                        UpdatePaintNodeData* data) {
  Q_UNUSED(data)

  const float width = this->width();
  const float height = this->height();

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

    auto* geometry = generateGeometry(radii);

    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

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

QSGGeometry* FlexRectangle::generateGeometry(const CornerRadii& radii) const {
  using Point2D = QSGGeometry::Point2D;

  const auto width = static_cast<float>(this->width());
  const auto height = static_cast<float>(this->height());

  std::vector<QSGGeometry::Point2D> vertexVec;
  vertexVec.reserve(8 * m_segments);

  for (int i = 0; i <= m_segments; i++) {
    double alpha = i * (M_PI_2 / static_cast<double>(m_segments));

    float x = radii.topLeft * (1 - cos(alpha));
    float y = radii.topLeft * (1 - sin(alpha));

    vertexVec.push_back(Point2D{.x = width / 2, .y = height / 2});
    vertexVec.push_back(Point2D({.x = x, .y = y}));
  }

  // Calculate TopRight vertices
  for (int i = 0; i <= m_segments; i++) {
    double alpha = i * (M_PI_2 / static_cast<double>(m_segments));

    float x = width - radii.topRight * (1 - sin(alpha));
    float y = radii.topRight * (1 - cos(alpha));

    vertexVec.push_back(Point2D{.x = width / 2, .y = height / 2});
    vertexVec.push_back(Point2D({.x = x, .y = y}));
  }

  // Calculate BottomRight vertices
  for (int i = 0; i <= m_segments; i++) {
    double alpha = i * (M_PI_2 / static_cast<double>(m_segments));

    float x = width - radii.bottomRight * (1 - cos(alpha));
    float y = height - radii.bottomRight * (1 - sin(alpha));

    vertexVec.push_back(Point2D{.x = width / 2, .y = height / 2});
    vertexVec.push_back(Point2D({.x = x, .y = y}));
  }

  // Calculate BottomLeft vertices
  for (int i = 0; i <= m_segments; i++) {
    double alpha = i * (M_PI_2 / static_cast<double>(m_segments));

    float x = radii.bottomLeft * (1 - sin(alpha));
    float y = height - radii.bottomLeft * (1 - cos(alpha));

    vertexVec.push_back(Point2D{.x = width / 2, .y = height / 2});
    vertexVec.push_back(Point2D({.x = x, .y = y}));
  }

  if (vertexVec.size() > 2) {
    vertexVec.push_back(vertexVec.at(1));
  }

  auto* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),
                                   vertexVec.size());

  geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);

  auto* vertices = geometry->vertexDataAsPoint2D();

  memcpy(vertices, vertexVec.data(),
         vertexVec.size() * sizeof(QSGGeometry::Point2D));

  return geometry;
}

} // namespace UI
