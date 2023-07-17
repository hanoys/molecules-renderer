#include "window.h"

#include <QApplication>
#include "my_math.h"
int main(int argc, char *argv[]) {
    mat4 mo;
    vec3 v{1, 2, 3};
    vec3 m = mat4::identity() * v;
    mat4 mm = mat4::identity() * mat4::identity();
    mat4 mat{0, 0, 0, 0, 1, 1, 1, 1};
    QApplication app(argc, argv);
    MainWindow window;
    window.showMaximized();
    return app.exec();
}
