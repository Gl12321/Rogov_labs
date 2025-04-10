#ifndef RAY_H
#define RAY_H

#include <QPoint>
#include <cmath>

class Ray {
public:
    Ray(const QPoint& begin, const QPoint& end, double angle)
        : begin_(begin), end_(end), angle_(angle) {}

    QPoint getBegin() const { return begin_; }
    QPoint getEnd() const { return end_; }
    double getAngle() const { return angle_; }

    void setBegin(const QPoint& begin) { begin_ = begin; }
    void setEnd(const QPoint& end) { end_ = end; }
    void setAngle(double angle) { angle_ = angle; }

    Ray Rotate(double angle) const {
        double new_angle = angle_ + angle;
        double length = std::sqrt(std::pow(end_.x() - begin_.x(), 2) + std::pow(end_.y() - begin_.y(), 2));
        int new_x = begin_.x() + static_cast<int>(length * std::cos(new_angle));
        int new_y = begin_.y() + static_cast<int>(length * std::sin(new_angle));
        return Ray(begin_, QPoint(new_x, new_y), new_angle);
    }

private:
    QPoint begin_;
    QPoint end_;
    double angle_;
};

#endif