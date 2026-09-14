#ifndef CALCULATIONTHREADMANAGER_H
#define CALCULATIONTHREADMANAGER_H

#include <QObject>
#include "calculationunion.h"
#include "calculationworker.h"
#include "calculationData.h"

class CalculationThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit CalculationThreadManager(QObject *parent = nullptr);
    int createWorker(const CalculationData &data, QWidget *windowParent = nullptr);
    void exitProcess();

private:
    CalculationUnion* findUnion(quint32 workerIndex);

private:
    QVector<CalculationUnion*> Workers;

signals:
    void workersVectorEmpty();

public slots:
    void deleteWorker(quint32 workerIndex);

};

#endif // CALCULATIONTHREADMANAGER_H
