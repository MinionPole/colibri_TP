#ifndef CALCULATIONWORKER_H
#define CALCULATIONWORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QDirIterator>
#include "calculationData.h"

class CalculationWorker : public QObject
{
    Q_OBJECT
public:
    explicit CalculationWorker(const CalculationData &data, QObject *parent = nullptr);

signals:
    void endOfWork();
    void refreshInfo(int nowVals, int potentialVals);
    void finished();

private:
    CalculationData data;
    QMutex sync;
    QWaitCondition pauseCond;
    bool paused = false;
    bool inStep = false;
    int onWork = false;
    QDirIterator* dirIterator = nullptr;
    QFile* currentFile = nullptr;
    QFile* resultFile = nullptr;
    QByteArray bufferFromFile;
    int fileNameCnt;
    int totalFiles, processedFiles;

public slots:
    void stopWorker();
    void pauseWorker();
    void resumeWorker();
    void startWorker();
    void exitWorker();
    void doWorkStep();
};

#endif // CALCULATIONWORKER_H
