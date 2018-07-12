#include "mainwindow.h"
#include "misc/config.h"
#include <QApplication>
#include <time.h>

QString base_url;
QString url_port;

MainWindow *w;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // For random number generator.
    srand((unsigned)time(0));

    Config cfg;
    cfg.loadSetting();

    w = new MainWindow;
    w->showMaximized();

    return a.exec();
}
