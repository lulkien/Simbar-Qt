#pragma once

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <cstdint>
#include <functional>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qquickrhiitem.h>
#include <qsggeometry.h>
#include <vector>

namespace UI {

/**
 * @class FlexRectangle
 * @brief A QQuickItem subclass that renders a rectangle with customizable
 * rounded corners.
 *
 * FlexRectangle is a custom QQuickItem that draws a rectangle with individually
 * configurable corner radii using the Qt Scene Graph. It supports dynamic
 * resizing, color changes, and adjustable corner smoothness via the number of
 * segments used for rendering arcs. The rectangle is drawn using a triangle
 * strip to form rounded corners, with the geometry regenerated whenever the
 * size, radius, or segments change.
 *
 * @property color The fill color of the rectangle.
 * @property radius A list of four corner radii [topLeft, topRight, bottomRight,
 * bottomLeft].
 * @property segments The number of segments used to approximate each rounded
 * corner.
 */
class FlexRectangle : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(
      QVariantList radius READ radius WRITE setRadius NOTIFY radiusChanged)
  Q_PROPERTY(
      uint32_t segments READ segments WRITE setSegments NOTIFY segmentsChanged)

public:
  /**
   * @brief Constructs a FlexRectangle with default properties.
   *
   * Initializes the item with a white color, 4-pixel radius for all corners,
   * and 8 segments per corner. Enables rendering by setting the ItemHasContents
   * flag.
   */
  FlexRectangle();

  /**
   * @brief Gets the fill color of the rectangle.
   * @return The current color as a QColor.
   */
  [[nodiscard]] QColor color() const { return m_color; }

  /**
   * @brief Sets the fill color of the rectangle.
   *
   * If the new color differs from the current one, updates the material, marks
   * the geometry as dirty, and triggers a redraw.
   *
   * @param color The new color to set.
   */
  void setColor(const QColor& color);

  /**
   * @brief Gets the corner radii of the rectangle.
   * @return A QVariantList containing [topLeft, topRight, bottomRight,
   * bottomLeft] radii.
   */
  [[nodiscard]] QVariantList radius() const { return m_radius; }

  /**
   * @brief Sets the corner radii of the rectangle.
   *
   * Accepts a QVariantList with 1 to 4 elements specifying corner radii in the
   * order [topLeft, topRight, bottomRight, bottomLeft]. If fewer than 4
   * elements are provided, missing values are set to 0. Marks the geometry as
   * dirty and triggers a redraw if changed.
   *
   * @param radius A QVariantList of corner radii.
   */
  void setRadius(const QVariantList& radius);

  /**
   * @brief Gets the number of segments used to approximate each rounded corner.
   * @return The number of segments as a uint32_t.
   */
  [[nodiscard]] uint32_t segments() const { return m_segments; }

  /**
   * @brief Sets the number of segments used to approximate each rounded corner.
   *
   * Higher segment counts result in smoother corners but increase rendering
   * cost. Marks the geometry as dirty and triggers a redraw if changed.
   *
   * @param newSegments The number of segments to use.
   */
  void setSegments(uint32_t newSegments);

  /**
   * @brief Updates the scene graph node for rendering the rectangle.
   *
   * Creates or updates a QSGGeometryNode with the current geometry and
   * material. Regenerates the geometry if marked dirty (due to size, radius, or
   * segment changes) and updates the material color if changed.
   *
   * @param oldNode The existing scene graph node, or nullptr if none exists.
   * @param data Update data (unused).
   * @return The updated QSGNode, or nullptr if the size is invalid.
   */
  QSGNode* updatePaintNode(QSGNode* oldNode,
                           UpdatePaintNodeData* data) override;

protected:
  /**
   * @brief Handles changes to the item's geometry (width or height).
   *
   * Marks the geometry as dirty and triggers a redraw if the size changes.
   *
   * @param newGeometry The new geometry of the item.
   * @param oldGeometry The previous geometry of the item.
   */
  void geometryChange(const QRectF& newGeometry,
                      const QRectF& oldGeometry) override;

signals:
  void colorChanged();    ///< Emitted when the color property changes.
  void radiusChanged();   ///< Emitted when the radius property changes.
  void segmentsChanged(); ///< Emitted when the segments property changes.

private:
  /**
   * @struct CornerRadii
   * @brief Stores the radii for each corner of the rectangle.
   */
  struct CornerRadii {
    float topLeft;
    float topRight;
    float bottomRight;
    float bottomLeft;

    /**
     * @brief Converts a QVariantList to corner radii.
     *
     * Parses a list of up to 4 elements to set corner radii in the order
     * [topLeft, topRight, bottomRight, bottomLeft]. If fewer elements are
     * provided, remaining radii are set to 0.
     *
     * @param list The QVariantList containing radius values.
     */
    void fromQVariantList(const QVariantList& list);

    /**
     * @brief Clamps corner radii to valid values.
     *
     * Ensures each radius is non-negative and does not exceed half the minimum
     * of the rectangle's width and height to prevent invalid geometry.
     *
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     */
    void clampRadius(const float& width, const float& height);
  };

  /**
   * @brief Generates the geometry for a rounded rectangle.
   *
   * Creates a QSGGeometry representing a rounded rectangle using a triangle
   * strip. For each corner with a non-zero radius, generates vertices along an
   * arc using the specified number of segments. The arcs are connected to form
   * a closed shape. The center point of the rectangle is used as the starting
   * vertex for each triangle strip segment to ensure proper filling.
   *
   * @param radii The corner radii for the rectangle.
   * @return A QSGGeometry object containing the vertex data.
   */
  [[nodiscard]] QSGGeometry* generateGeometry(const CornerRadii& radii) const;

  void generateCornerVertices(
      std::vector<QSGGeometry::Point2D>& vertices, const float& radius,
      const std::function<QSGGeometry::Point2D(float, float)>& equation) const;

  bool m_geometryDirty = true;
  uint32_t m_segments = 8;
  QColor m_color = Qt::white;
  QVariantList m_radius = {4, 4, 4, 4};
};

} // namespace UI
