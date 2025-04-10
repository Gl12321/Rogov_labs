#include "mainwindow.h"
#include <QtWidgets/QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), mode_(LIGHT), drawing_polygon_(false) {
    QWidget* central_widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central_widget);
    mode_selector_ = new QComboBox(this);
    mode_selector_->addItem("Light");
    mode_selector_->addItem("Polygons");
    drawing_area_ = new QWidget(this);
    drawing_area_->setMinimumSize(800, 600);
    layout->addWidget(mode_selector_);
    layout->addWidget(drawing_area_);
    setCentralWidget(central_widget);

    std::vector<QPoint> boundary = {QPoint(0, 0), QPoint(800, 0), QPoint(800, 600), QPoint(0, 600)};
    controller_.AddPolygon(Polygon(boundary));

    QObject::connect(mode_selector_, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     this, &MainWindow::updateMode);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (mode_ == LIGHT) {
        Polygon light_area = controller_.CreateLightArea();
        painter.setBrush(QColor(255, 255, 0, 50));
        painter.setPen(Qt::NoPen);
        for (size_t i = 0; i < light_area.getVertices().size(); ++i) {
            QPoint p1 = light_area.getVertices()[i];
            QPoint p2 = light_area.getVertices()[(i + 1) % light_area.getVertices().size()];
            painter.drawLine(p1, p2);
        }
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(controller_.getLightSource(), 5, 5);
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    for (const auto& poly : controller_.GetPolygons()) {
        for (size_t i = 0; i < poly.getVertices().size(); ++i) {
            QPoint p1 = poly.getVertices()[i];
            QPoint p2 = poly.getVertices()[(i + 1) % poly.getVertices().size()];
            painter.drawLine(p1, p2);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (mode_ == POLYGONS) {
        if (event->button() == Qt::LeftButton) {
            if (!drawing_polygon_) {
                controller_.AddPolygon(Polygon({event->pos()}));
                drawing_polygon_ = true;
            } else {
                controller_.AddVertexToLastPolygon(event->pos());
            }
            update();
        } else if (event->button() == Qt::RightButton) {
            drawing_polygon_ = false;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (mode_ == LIGHT) {
        controller_.setLightSource(event->pos());
        update();
    } else if (mode_ == POLYGONS && drawing_polygon_) {
        controller_.UpdateLastPolygon(event->pos());
        update();
    }
}

void MainWindow::updateMode(int index) {
    mode_ = (index == 0) ? LIGHT : POLYGONS;
    drawing_polygon_ = false;
    update();
}