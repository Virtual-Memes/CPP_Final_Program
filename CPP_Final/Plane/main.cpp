#include "mainwindow.h"

#include <QApplication>
int main(int argc, char *argv[])
{
    //创建QApplication对象
    QApplication a(argc, argv);
    //创建窗口对象并显示
    MainWindow w;
    w.show();
    return a.exec();
}
