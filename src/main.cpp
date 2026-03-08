#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("MovieInfo");
    app.setOrganizationName("MovieInfo");

    UI::MainWindow window;
    window.resize(1400, 800);
    window.show();

    return app.exec();
}
