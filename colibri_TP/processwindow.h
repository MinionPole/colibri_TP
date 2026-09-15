#ifndef PROCESSWINDOW_H
#define PROCESSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include "calculationData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class processWindow; }
QT_END_NAMESPACE

class processWindow : public QWidget
{
    Q_OBJECT
public:
    explicit processWindow(quint32 workerIndex, QWidget *parent = nullptr);
    ~processWindow() override;
    QPushButton* getButton(int ind);
    void setBaseInfo(const CalculationData& data);

public:
    quint32 workerIndex;

public slots:
    void refreshInfo(int nowVals, int potentialVals, int flagOfWork, double PercentOfFile);
    void resetTimer(int time);
    void updateTimerEdit();


private:
    Ui::processWindow *ui;
    QTimer *timer;
};

#endif // PROCESSWINDOW_H
