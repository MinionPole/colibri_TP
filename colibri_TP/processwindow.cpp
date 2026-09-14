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

void processWindow::refreshInfo(int nowVals, int potentialVals){

};
