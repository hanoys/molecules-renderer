#pragma once

#include <QMainWindow>
#include <scene.h>
#include <QGraphicsScene>

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
  void draw();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<QGraphicsScene> graphics_scene;
  std::shared_ptr<Scene> scene;
};
