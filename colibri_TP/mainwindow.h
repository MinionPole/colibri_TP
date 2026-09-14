#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QIntValidator>
#include "calculationworker.h"
#include "calculationData.h"
#include "calculationthreadmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QString initialPath;
    QString resultPath;
    CalculationThreadManager TManager;
    int wantExit;

private slots:
    void selectInitialFolderClick();
    void selectResultFolderClick();
    void startProcess();
    void exitMainMenu();
    void exitChecker();
};
#endif // MAINWINDOW_H
