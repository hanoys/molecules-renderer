#include "window.h"
#include "ui_mainwindow.h"

#include "graphics.h"
#include "model.h"
#include <iostream>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setup_scene();
  draw();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setup_scene() {
    graphics_scene = std::make_shared<QGraphicsScene>(this);
    ui->graphicsView->setScene(graphics_scene.get());
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    auto cont = ui->graphicsView->contentsRect();
    graphics_scene->setSceneRect(0, 0, cont.width(), cont.height());
    scene = std::make_shared<Scene>();
}

void MainWindow::draw() {
    //scene->draw_triangle(QVector2D{200, 200}, QVector2D{200, 500}, QVector2D{600, 100}, qRgb(255, 255, 255));
    Model model{"../model/cube.txt"};
    model.rotate(mat4::rotate_x(-0.5));
    model.rotate(mat4::rotate_y(0.2));
    model.rotate(mat4::rotate_z(0.4));
    scene->add_model(model);
    scene->draw_models2();
    graphics_scene->addPixmap(QPixmap::fromImage(scene->getImage()));
}
