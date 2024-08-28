#include "mainwindow.h"

#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setMaximumWidth(500);
    w.setMaximumHeight(200);

    Controller *control = new Controller(&w);

    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(control);

    QWidget *central_widget = new QWidget(&w);
    central_widget->setLayout(h_layout);

    w.setCentralWidget(central_widget);

    w.show();

    return a.exec();
}
