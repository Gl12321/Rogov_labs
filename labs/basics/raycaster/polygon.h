#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <optional>
#include <QPoint>
#include "ray.h"

class Polygon {
public:
    Polygon(const std::vector<QPoint>& vertices) : vertices_(vertices) {}

    const std::vector<QPoint>& getVertices() const { return vertices_; }
    void AddVertex(const QPoint& vertex) { vertices_.push_back(vertex); }
    void UpdateLastVertex(const QPoint& new_vertex) {
        if (!vertices_.empty()) {
            vertices_.back() = new_vertex;
        }
    }

    std::optional<QPoint> IntersectRay(const Ray& ray) const {
        std::optional<QPoint> closest_intersection = std::nullopt;
        double min_dist = std::numeric_limits<double>::max();

        for (size_t i = 0; i < vertices_.size(); ++i) {
            QPoint p1 = vertices_[i];
            QPoint p2 = vertices_[(i + 1) % vertices_.size()];

            if (auto intersection = lineSegmentIntersection(ray.getBegin(), ray.getEnd(), p1, p2)) {
                double dist = distance(ray.getBegin(), *intersection);
                if (dist < min_dist) {
                    min_dist = dist;
                    closest_intersection = intersection;
                }
            }
        }
        return closest_intersection;
    }

private:
    std::vector<QPoint> vertices_;

    double distance(const QPoint& p1, const QPoint& p2) const {
        return std::sqrt(std::pow(p2.x() - p1.x(), 2) + std::pow(p2.y() - p1.y(), 2));
    }

    std::optional<QPoint> lineSegmentIntersection(const QPoint& a1, const QPoint& a2,
                                                  const QPoint& b1, const QPoint& b2) const {
        double x1 = a1.x(), y1 = a1.y();
        double x2 = a2.x(), y2 = a2.y();
        double x3 = b1.x(), y3 = b1.y();
        double x4 = b2.x(), y4 = b2.y();

        double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (denom == 0) return std::nullopt;

        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            int x = x1 + t * (x2 - x1);
            int y = y1 + t * (y2 - y1);
            return QPoint(x, y);
        }
        return std::nullopt;
    }
};

#endif