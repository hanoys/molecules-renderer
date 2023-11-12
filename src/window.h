#pragma once

#include <QMainWindow>
#include <scene.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "scene.h"

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
  std::shared_ptr<Scene> scene;
  Model *model;
  Model *second_model;
  QGraphicsPixmapItem *pixmap;

private slots:
    void draw();
};
