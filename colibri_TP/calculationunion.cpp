#include "calculationunion.h"

CalculationUnion::CalculationUnion(quint32 index, const CalculationData &data,
                                   QWidget *windowParent,
                                   QObject *parent)
    : QObject(parent)
    , workerIndex(index)
    , processWindowObj(new processWindow(index, windowParent))
    , worker(data)
{
    worker.moveToThread(&executionThread);
    this->processWindowObj->setBaseInfo(data);

    connect(processWindowObj->getButton(0), &QPushButton::clicked, &worker, &CalculationWorker::startWorker);
    connect(processWindowObj->getButton(1), &QPushButton::clicked, &worker, &CalculationWorker::pauseWorker);
    connect(processWindowObj->getButton(2), &QPushButton::clicked, &worker, &CalculationWorker::resumeWorker,Qt::DirectConnection);
    connect(processWindowObj->getButton(3), &QPushButton::clicked, &worker, &CalculationWorker::stopWorker);
    connect(processWindowObj->getButton(4), &QPushButton::clicked, &worker, &CalculationWorker::exitWorker);

    connect(&worker, &CalculationWorker::refreshInfo, processWindowObj, &processWindow::refreshInfo);

    connect(&worker, &CalculationWorker::finished, &executionThread, &QThread::quit);


    connect(&executionThread, &QThread::finished, this, [this]() {

        worker.deleteLater();
        emit this->deleteWorker(this->workerIndex);
        //this->deleteLater();
    });

    executionThread.start();
}

CalculationUnion::~CalculationUnion()
{
    if (executionThread.isRunning()) {
        executionThread.quit();
        executionThread.wait();
    }
}