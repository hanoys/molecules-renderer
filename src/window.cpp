#include "window.h"
#include "ui_mainwindow.h"

#include "graphics.h"
#include "model.h"
#include <iostream>
#include <QColor>
#include <chrono>
#include <thread>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  setup_scene();
  QTimer *t = new QTimer(this);
  connect(t, SIGNAL(timeout()), this, SLOT(draw()));
  t->setInterval(20);
  t->start();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setup_scene() {
    graphics_scene = std::make_shared<QGraphicsScene>(this);
    ui->graphicsView->setScene(graphics_scene.get());
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    auto cont = ui->graphicsView->contentsRect();
    graphics_scene->setSceneRect(0, 0, cont.width(), cont.height());
    scene = std::make_shared<Scene>();
//    Model monkey{"../model/monkey"};
//    Mesh monkey_mesh{"../model/monkey"};
//    scene->add_mesh(monkey_mesh);
//    scene->add_model(monkey);
    Model first{"../model/sphere.obj"};
    first.translate_to_center();
    scene->add_model(first);
//    Model m{"../model/cube.txt"};
//    scene->add_model(m);


    Model cyl = scene->cylinder(30, 0.1f, 10);
    cyl.translate_to_center();
    scene->add_model(cyl);

    pixmap = graphics_scene->addPixmap(QPixmap::fromImage(scene->getImage()));
}

void MainWindow::draw() {
    scene->getImage().fill(QColor(0, 0, 0));
    scene->draw_models();
    pixmap->setPixmap(QPixmap::fromImage(scene->getImage()));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            for (auto &mesh : scene->models) {
//                mesh.rotate(mat4f::rotate_x(-0.02f));
                mesh.translate({0, 0, -0.2f});
            }
            break;
        case Qt::Key_S:
            for (auto &mesh : scene->models) {
//                mesh.rotate(mat4f::rotate_x(0.02f));
                mesh.translate({0, 0, 0.2f});
            }
            break;
        case Qt::Key_D:
            for (auto &mesh : scene->models) {
                mesh.translate({0.2f, 0, 0});
            }
            break;
        case Qt::Key_A:
            for (auto &mesh : scene->models) {
                mesh.translate({-0.2f, 0, 0});
            }
            break;
        case Qt::Key_R:
            for (auto &mesh : scene->models) {
                mesh.rotate(mat4f::rotate_y(0.02f));
            }
            break;
        case Qt::Key_Q:
            for (auto &mesh : scene->models) {
                mesh.rotate(mat4f::rotate_y(-0.02f));
            }
            break;
    }
}
