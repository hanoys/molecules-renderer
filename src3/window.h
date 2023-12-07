#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "engine/renderer.h"
#include "engine/loader.h"

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

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QGraphicsScene> graphics_scene;
    std::shared_ptr<QImage> image;
    QGraphicsPixmapItem *pixmap;


    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Mesh> mesh;

    int draw_mode = 1;
private slots:

    void draw();
};