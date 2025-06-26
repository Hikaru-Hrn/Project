#ifndef CPUREADER_H
#define CPUREADER_H

#include <QMap>
#include <QString>

class CpuReader {
public:
    static QMap<QString, double> readTemperatures(bool useFakeData = false);
};

#endif // CPUREADER_H
