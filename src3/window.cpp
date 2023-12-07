#include "window.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QColor>
#include <thread>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

#include "engine/loader.h"

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

    Loader loader;
    mesh = std::make_shared<Mesh>();
    loader.load_model(mesh, "../model/sphere3.obj");
    mesh->translate(-mesh->center);

    image = std::make_shared<QImage>(1080, 720, QImage::Format_ARGB32);
    pixmap = graphics_scene->addPixmap(QPixmap::fromImage(*image));

    camera = std::make_shared<Camera>();
    camera->pos = m3::vec3{0, 0, -10};
    camera->target = m3::vec3{0, 0, 0};
    camera->up = m3::vec3{0, 1, 0};
    camera->view_matrix = m3::look_at(camera->pos, camera->target, camera->up);
    camera->projection_matrix = m3::perspective(45, (float) image->width() / (float) image->height(), 0.1f, 10.f);
    camera->scale_matrix = m3::scale({2, -2, 2});
    renderer = std::make_shared<Renderer>(image, camera);

}

void MainWindow::draw() {
    image->fill(QColor(34,34,51));
    m3::mat4 cam_rot = m3::rotate_y(0.005f);
    camera->pos = m3::transform_vector(cam_rot, camera->pos);
    camera->up = m3::transform_vector(cam_rot, camera->up);
    camera->view_matrix = m3::look_at(camera->pos, camera->target, camera->up);
    renderer->draw_mesh(mesh);
    pixmap->setPixmap(QPixmap::fromImage(*image));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
        case Qt::Key_W:
//                mesh->translate({0, 0, -0.2f});
                mesh->rotate(m3::rotate_x(-0.1f));
//                mesh->rotate(m3::rotate_y(-0.1f));
//                mesh->translate({0.2f, 0, 0});
//                mesh->translate({0, 0.2f, 0});
            break;
        case Qt::Key_S:
//                mesh->translate({0, 0, 0.2f});
//                mesh->translate({-0.2f, 0, 0});
                mesh->rotate(m3::rotate_x(0.1f));
//                mesh->rotate(m3::rotate_y(0.1f));
//                mesh->translate({0, -0.2f, 0});
            break;
        case Qt::Key_D:
                mesh->rotate(m3::rotate_y(-0.1f));
//                mesh->translate({-0.2f, 0, 0});
            break;
        case Qt::Key_A:
                mesh->rotate(m3::rotate_y(0.1f));
//                mesh->translate({0.2f, 0, 0});
            break;
        case Qt::Key_M:
            draw_mode = (draw_mode + 1) % 3 + 1;
    }
}
