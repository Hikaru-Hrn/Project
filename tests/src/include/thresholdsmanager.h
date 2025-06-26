#ifndef THRESHOLDSMANAGER_H
#define THRESHOLDSMANAGER_H

#include <QMap>
#include <QString>
#include <QSettings>

struct SensorThreshold {
    int min;
    int max;
    
    SensorThreshold() : min(30), max(70) {}
    SensorThreshold(int minVal, int maxVal) : min(minVal), max(maxVal) {}
    
    bool isValid() const { return min < max; }
};

class ThresholdsManager {
public:
    ThresholdsManager();
    
    SensorThreshold getThreshold(const QString &sensor) const;  // Теперь const
    void setThreshold(const QString &sensor, const SensorThreshold &threshold);
    void save();
    void load();

private:
    QMap<QString, SensorThreshold> data;
    QSettings settings;
};

#endif // THRESHOLDSMANAGER_H
