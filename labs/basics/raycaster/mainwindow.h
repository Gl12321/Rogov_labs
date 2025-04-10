#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>
#include "controller.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void updateMode(int index);

    Controller controller_;
    QComboBox* mode_selector_;
    QWidget* drawing_area_;
    enum Mode { LIGHT, POLYGONS } mode_;
    bool drawing_polygon_;
};

#endif