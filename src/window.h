#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "engine/renderer.h"
#include "engine/loader.h"
#include "engine/scene.h"
#include "engine/render_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:
    void setup_scene();

    void keyPressEvent(QKeyEvent *event);
    m3::vec3 meshes_center(std::vector<Mesh> meshes);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QGraphicsScene> graphics_scene;
    std::shared_ptr<QImage> image;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<RenderManager> renderManager;
    QGraphicsPixmapItem *pixmap;

    m3::vec3 molecula_center = {0, 0, 0};

    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Mesh> mesh;
private slots:
    void draw();
    void flat_button_toggled(bool checked);
    void gourand_button_toggled(bool checked);
    void phong_button_toggled(bool checked);
    void start_button_pressed();
};