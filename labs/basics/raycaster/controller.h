#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "polygon.h"

class Controller {
public:
    Controller() : light_source_(QPoint(0, 0)) {}

    const std::vector<Polygon>& GetPolygons() const { return polygons_; }
    void AddPolygon(const Polygon& polygon) { polygons_.push_back(polygon); }
    void AddVertexToLastPolygon(const QPoint& new_vertex) {
        if (!polygons_.empty()) {
            polygons_.back().AddVertex(new_vertex);
        }
    }
    void UpdateLastPolygon(const QPoint& new_vertex) {
        if (!polygons_.empty()) {
            polygons_.back().UpdateLastVertex(new_vertex);
        }
    }

    QPoint getLightSource() const { return light_source_; }
    void setLightSource(const QPoint& source) { light_source_ = source; }

private:
    std::vector<Polygon> polygons_;
    QPoint light_source_;
};

#endif