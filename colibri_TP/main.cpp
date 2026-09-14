#include "mainwindow.h"

#include <QApplication>
#include "calculationData.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<CalculationData>("CalculationData");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
