#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QScrollArea>
#include <QVBoxLayout>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int width=2000;
    int height=700;
    w.resize(width,height);
    w.setFixedSize(width,height);
    QScrollArea scrollArea;
    scrollArea.setWidget(&w);
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea.setWidgetResizable(true);
    int wid=1000;
    int heig=700;
    scrollArea.resize(wid,heig);
    scrollArea.setWindowTitle("Программа вычисления периода сигнала");
    scrollArea.show();
    return a.exec();
}


