#include "calculationthreadmanager.h"

CalculationThreadManager::CalculationThreadManager(QObject *parent)
    : QObject{parent}
{

}

void CalculationThreadManager::deleteWorker(quint32 workerIndex){

    CalculationUnion* toDelete = findUnion(workerIndex);
    if(toDelete != nullptr){
        toDelete->processWindowObj->hide();
        Workers.removeOne(toDelete);
        delete toDelete;
    }
    if(Workers.size() == 0)
        emit  workersVectorEmpty();
};

CalculationUnion* CalculationThreadManager::findUnion(quint32 workerIndex){
    for (CalculationUnion* unionPtr : Workers) {
        if (unionPtr->workerIndex == workerIndex) {
            return unionPtr;
        }
    }
    return nullptr;
};

int CalculationThreadManager::createWorker(const CalculationData &data,
                                           QWidget *windowParent)
{
    const int index = Workers.size();

    auto *u = new CalculationUnion(index, data, windowParent, this);
    connect(u, &CalculationUnion::deleteWorker, this, &CalculationThreadManager::deleteWorker);
    Workers.push_back(u);
    u->processWindowObj->show();

    return index;
}

void CalculationThreadManager::exitProcess(){
    if(Workers.size() == 0)
        emit  workersVectorEmpty();
    else
        for (CalculationUnion* unionPtr : Workers) {
            unionPtr->processWindowObj->getButton(4)->click();
        }
};