#include "flexrectangle.h"

#include <QSGGeometry>
#include <QSGVertexColorMaterial>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <qassert.h>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qminmax.h>
#include <qsggeometry.h>
#include <qtpreprocessorsupport.h>
#include <qvariant.h>
#include <vector>

namespace UI {

/**
 * @brief Converts a QVariantList to CornerRadii values.
 *
 * Sets the topLeft, topRight, bottomRight, and bottomLeft radii from a
 * QVariantList. If the list has fewer than 4 elements, remaining radii are set
 * to 0. Uses a lambda to safely convert QVariant values to floats.
 *
 * @param list The QVariantList containing radius values.
 */
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

/**
 * @brief Clamps corner radii to valid values.
 *
 * Ensures each radius is non-negative and does not exceed half the minimum
 * of the rectangle's width and height to prevent invalid geometry.
 *
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 */
void FlexRectangle::CornerRadii::clampRadius(const float& width,
                                             const float& height) {
  float maxRadius = qMin(width / 2, height / 2);

  topLeft = qMax(0.0F, qMin(maxRadius, topLeft));
  topRight = qMax(0.0F, qMin(maxRadius, topRight));
  bottomRight = qMax(0.0F, qMin(maxRadius, bottomRight));
  bottomLeft = qMax(0.0F, qMin(maxRadius, bottomLeft));
}

// ###################################################################################

/**
 * @brief Constructs a FlexRectangle with default properties.
 */
FlexRectangle::FlexRectangle() { this->setFlag(ItemHasContents, true); }

/**
 * @brief Sets the fill color of the rectangle.
 */
void FlexRectangle::setColor(const QColor& color) {
  if (m_color == color) {
    return;
  }
  m_color = color;
  emit colorChanged();
  update();
}

/**
 * @brief Sets the corner radii of the rectangle.
 */
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

/**
 * @brief Sets the number of segments for corner arcs.
 */
void FlexRectangle::setSegments(uint32_t newSegments) {
  if (m_segments == newSegments) {
    return;
  }

  m_segments = qMax(1U, newSegments);
  m_geometryDirty = true;
  emit segmentsChanged();
  update();
}

/**
 * @brief Updates the scene graph node for rendering.
 */
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

/**
 * @brief Handles geometry changes to trigger redraws.
 */
void FlexRectangle::geometryChange(const QRectF& newGeometry,
                                   const QRectF& oldGeometry) {
  if (newGeometry.size() != oldGeometry.size()) {
    m_geometryDirty = true;
    update();
  }
  QQuickItem::geometryChange(newGeometry, oldGeometry);
}

/**
 * @brief Generates the geometry for a rounded rectangle.
 *
 * This function creates a QSGGeometry object representing a rounded rectangle
 * using a triangle strip drawing mode. The rectangle is constructed by
 * generating vertices for each corner arc (if the radius is non-zero) and
 * connecting them to form a closed shape. The process is as follows:
 *
 * 1. **Vertex Generation**:
 *    - For each corner (top-left, top-right, bottom-right, bottom-left),
 * generate `m_segments + 1` vertices along a 90-degree arc if the corner radius
 * is non-zero.
 *    - Each arc is approximated using a series of points calculated with
 * trigonometric functions (cos and sin) to form a smooth curve.
 *    - The center of the rectangle (width/2, height/2) is used as the starting
 * vertex for each triangle strip segment to ensure the shape is filled
 * correctly.
 *    - Vertices are generated in a clockwise order starting from the top-left
 * corner.
 *
 * 2. **Arc Calculation**:
 *    - For each corner, the angle `alpha` ranges from 0 to π/2 (90 degrees) in
 * increments of π/2 / m_segments.
 *    - The x and y coordinates are calculated using the corner radius and
 * trigonometric functions, offset to position the arc correctly relative to the
 * corner.
 *
 * 3. **Closing the Shape**:
 *    - If the vertex list contains more than two vertices, the first vertex
 * after the initial center point is appended to close the triangle strip loop.
 *
 * 4. **Geometry Creation**:
 *    - A QSGGeometry object is created with the calculated number of vertices.
 *    - The drawing mode is set to QSGGeometry::DrawTriangleStrip to render the
 * shape.
 *    - The vertex data is copied into the geometry object.
 *
 * @param radii The CornerRadii structure containing the radius for each corner.
 * @return A QSGGeometry object containing the vertex data for the rounded
 * rectangle.
 */
QSGGeometry* FlexRectangle::generateGeometry(const CornerRadii& radii) const {
  using Point2D = QSGGeometry::Point2D;

  const auto width = static_cast<float>(this->width());
  const auto height = static_cast<float>(this->height());

  std::vector<QSGGeometry::Point2D> vertexVec;
  vertexVec.reserve(8 * m_segments);

  generateCornerVertices(vertexVec, radii.topLeft,
                         [](float radius, float angle) -> Point2D {
                           return Point2D{
                               .x = radius * (1 - std::cos(angle)),
                               .y = radius * (1 - std::sin(angle)),
                           };
                         });

  generateCornerVertices(
      vertexVec, radii.topRight,
      [width](const float& radius, const float& alpha) -> Point2D {
        return Point2D{.x = width - radius * (1 - std::sin(alpha)),
                       .y = radius * (1 - std::cos(alpha))};
      });

  generateCornerVertices(
      vertexVec, radii.bottomRight,
      [width, height](const float& radius, const float& alpha) -> Point2D {
        return Point2D{.x = width - radius * (1 - std::cos(alpha)),
                       .y = height - radius * (1 - std::sin(alpha))};
      });

  generateCornerVertices(
      vertexVec, radii.bottomLeft,
      [height](const float& radius, const float& alpha) -> Point2D {
        return Point2D{.x = radius * (1 - std::sin(alpha)),
                       .y = height - radius * (1 - std::cos(alpha))};
      });

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

void FlexRectangle::generateCornerVertices(
    std::vector<QSGGeometry::Point2D>& vertices, const float& radius,
    const std::function<QSGGeometry::Point2D(float, float)>& equation) const {

  auto width = static_cast<float>(this->width());
  auto height = static_cast<float>(this->height());

  auto step = qMin(m_segments, static_cast<uint32_t>(radius) + 1);
  Q_ASSERT(step != 0);

  for (int i = 0; i <= step; i++) {
    auto alpha = static_cast<float>(i * (M_PI_2 / step));
    vertices.push_back(QSGGeometry::Point2D{.x = width / 2, .y = height / 2});
    vertices.push_back(equation(radius, alpha));
  }
}

} // namespace UI
