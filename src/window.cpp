#include "window.h"
#include "ui_mywindow.h"

#include <QColor>
#include <thread>
#include <QTimer>
#include <QKeyEvent>

#include "engine/loader.h"
#include "visualization/molecula_generator.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setup_scene();
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(draw()));
    connect(ui->flat_button, &QRadioButton::toggled, this, &MainWindow::flat_button_toggled);
    connect(ui->gourand_button, &QRadioButton::toggled, this, &MainWindow::gourand_button_toggled);
    connect(ui->phong_button, &QRadioButton::toggled, this, &MainWindow::phong_button_toggled);
    connect(ui->start_button, &QPushButton::pressed, this, &MainWindow::start_button_pressed);
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

    image = std::make_shared<QImage>(1300, 840, QImage::Format_ARGB32);
    pixmap = graphics_scene->addPixmap(QPixmap::fromImage(*image));

    Camera cam(m3::vec3{0, 0, -10},
                                      m3::vec3{0, 0, 0},
                                      m3::vec3{0, 1, 0},
                                      45,
                                      (float) image->width() / (float) image->height());
    cam.scale_matrix = m3::scale({1, -1, 1});

    scene = std::make_shared<Scene>(cam);
    scene->add_light(m3::normalized({-0.8f, 0.f, -1.f}));
    scene->add_light(m3::normalized({0.f, 0.f, 1.f}));
    renderManager = std::make_shared<RenderManager>(image, scene);
    std::shared_ptr<IShader> shader = std::make_shared<PhongShader>();
    renderManager->set_shader(shader);
}

void MainWindow::draw() {
    image->fill(QColor(34,34,51));

    renderManager->render();
    pixmap->setPixmap(QPixmap::fromImage(*image));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    m3::mat4 cam_rot;
    m3::vec3 delta;
    switch (event->key()) {
        case Qt::Key_W:
            scene->rotate_meshes_around_point(m3::rotate_x(-0.1f), molecula_center);
            break;
        case Qt::Key_S:
            scene->rotate_meshes_around_point(m3::rotate_x(0.1f), molecula_center);
            break;
        case Qt::Key_D:
            scene->rotate_meshes_around_point(m3::rotate_y(-0.1f), molecula_center);
            break;
        case Qt::Key_A:
            scene->rotate_meshes_around_point(m3::rotate_y(0.1f), molecula_center);
            break;
        case Qt::Key_Up:
            delta = {0, 0.1f, 0};
            scene->translate_meshes(delta);
            molecula_center = molecula_center + delta;
            break;
        case Qt::Key_Down:
            delta = {0, -0.1f, 0};
            scene->translate_meshes(delta);
            molecula_center = molecula_center + delta;
            break;
        case Qt::Key_Right:
            delta = {-0.1f, 0, 0};
            scene->translate_meshes(delta);
            molecula_center = molecula_center + delta;
            break;
        case Qt::Key_Left:
            delta = {0.1f, 0, 0};
            scene->translate_meshes(delta);
            molecula_center = molecula_center + delta;
            break;
        case Qt::Key_Z:
            scene->zoom_camera(1.01f);
            break;
        case Qt::Key_X:
            scene->zoom_camera(0.99f);
            break;
    }
}

void MainWindow::start_button_pressed() {
    std::string formula = ui->formula_line->text().toStdString();
    scene->clear_meshes();

    MoleculaGenerator g;
    g.init();
    g.parse_string(formula);
    for (auto &mesh : g.meshes)
        scene->add_mesh(mesh);
    molecula_center = meshes_center(g.meshes);
}

void MainWindow::flat_button_toggled(bool checked) {
    if (checked) {
        std::shared_ptr<IShader> s = std::make_shared<FlatShader>();
        renderManager->set_shader(s);
    }
}

void MainWindow::gourand_button_toggled(bool checked) {
    if (checked) {
        std::shared_ptr<IShader> s = std::make_shared<GourandShader>();
        renderManager->set_shader(s);
    }
}

void MainWindow::phong_button_toggled(bool checked) {
    if (checked) {
        std::shared_ptr<IShader> s = std::make_shared<PhongShader>();
        renderManager->set_shader(s);
    }
}

m3::vec3 MainWindow::meshes_center(std::vector<Mesh> meshes) {
    float min_x = meshes[0].verts[0].x;
    float max_x = meshes[0].verts[0].x;
    float min_y = meshes[0].verts[0].y;
    float max_y = meshes[0].verts[0].y;
    float min_z = meshes[0].verts[0].z;
    float max_z = meshes[0].verts[0].z;

    for (auto &mesh : meshes) {
        for (auto &vertex : mesh.verts) {
            if (vertex.x < min_x)
                min_x = vertex.x;
            else if (vertex.x > max_x)
                max_x = vertex.x;
            if (vertex.y < min_y)
                min_y = vertex.y;
            else if (vertex.y > max_y)
                max_y = vertex.y;
            if (vertex.z < min_z)
                min_z = vertex.z;
            else if (vertex.z > max_z)
                max_z = vertex.z;
        }
    }

    return {(min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2};
}