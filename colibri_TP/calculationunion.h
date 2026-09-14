#ifndef CALCULATIONUNION_H
#define CALCULATIONUNION_H

#include <QObject>
#include <QThread>
#include <QPushButton>
#include "processwindow.h"
#include "calculationworker.h"
#include "calculationdata.h"

class CalculationUnion : public QObject
{
    Q_OBJECT
public:
    explicit CalculationUnion(quint32 index, const CalculationData &data,
                              QWidget *windowParent = nullptr,
                              QObject *parent = nullptr);
    ~CalculationUnion() override;

    quint32 workerIndex;
    processWindow *processWindowObj = nullptr;
    QThread executionThread;
    CalculationWorker worker;

signals:
    void deleteWorker(quint32 workerIndex);

};

#endif // CALCULATIONUNION_H
