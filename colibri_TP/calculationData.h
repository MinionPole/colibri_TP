#ifndef CALCULATIONDATA_H
#define CALCULATIONDATA_H

#include <QString>
#include <cstdint>
#include <QRegularExpression>

struct CalculationData {
    QString initialPath;
    QString resultPath;
    std::uint64_t xorKey;
    QRegularExpression fileExpr;
    bool deleteInitial;
    bool timerWork;
    int timerTime;
};

#endif // CALCULATIONDATA_H
