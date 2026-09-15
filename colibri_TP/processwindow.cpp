#include "processwindow.h"
#include "./ui_processwindow.h"

processWindow::processWindow(quint32 workerIndex, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::processWindow)
    , workerIndex(workerIndex)
{
    ui->setupUi(this);
    setWindowTitle(QString("Process %1").arg(workerIndex));
    resize(400, 300);
}

processWindow::~processWindow()
{
    delete ui;
}

QPushButton* processWindow::getButton(int ind){
    if(ind == 0)
        return this->ui->startExecButton;
    if(ind == 1)
        return this->ui->pauseExecButton;
    if(ind == 2)
        return this->ui->resumeExecButton;
    if(ind == 3)
        return this->ui->stopProcessButton;
    if(ind == 4)
        return this->ui->exitButton;
    return nullptr;
};

void processWindow::setBaseInfo(const CalculationData& data){
    this->ui->RegExpLabel->setText(data.fileExpr.pattern());
    ui->adressFromLabel->setText(data.initialPath.last(qMin(data.initialPath.length(), 30)));
    ui->adressToLabel->setText(data.resultPath.last(qMin(data.resultPath.length(), 30)));
    ui->xorValLabel->setText(QString::number(data.xorKey));
};

void processWindow::refreshInfo(int nowVals, int potentialVals, int flagOfWork, double PercentOfFile){
    ui->alreadeMadeLabel->setText(QString("%1 из %2").arg(nowVals).arg(potentialVals));
    ui->thsFileBar->setValue(static_cast<int>(PercentOfFile * 100.0));

    switch (flagOfWork) {
    case 0:
        ui->alreadeMadeLabel_2->setText("Перед работой");
        break;

    case 1:
        ui->alreadeMadeLabel_2->setText("В работе");
        break;

    case 2:
        ui->alreadeMadeLabel_2->setText("На паузе");
        break;

    case 3:
        ui->alreadeMadeLabel_2->setText("Завершен");
        break;

    case 4:
        ui->alreadeMadeLabel_2->setText("В ожидании таймера");
        break;

    default:
         ui->alreadeMadeLabel_2->setText("Завершен");
        break;
    }
};
