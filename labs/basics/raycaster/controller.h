#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "polygon.h"
#include "ray.h"

class Controller {
public:
    Controller() : light_source_(0, 0) {}

    const std::vector<Polygon>& GetPolygons() const { return polygons_; }
    void AddPolygon(const Polygon& polygon) { polygons_.push_back(polygon); }
    void AddVertexToLastPolygon(const QPointF& new_vertex) {
        if (!polygons_.empty()) {
            polygons_.back().AddVertex(new_vertex);
        }
    }
    void UpdateLastPolygon(const QPointF& new_vertex) {
        if (!polygons_.empty()) {
            polygons_.back().UpdateLastVertex(new_vertex);
        }
    }

    QPointF getLightSource() const { return light_source_; }
    void setLightSource(const QPointF& source) { light_source_ = source; }

    std::vector<Ray> CastRays();
    void IntersectRays(std::vector<Ray>* rays);
    void RemoveAdjacentRays(std::vector<Ray>* rays);
    Polygon CreateLightArea();

private:
    std::vector<Polygon> polygons_;
    QPointF light_source_;
};

#endif