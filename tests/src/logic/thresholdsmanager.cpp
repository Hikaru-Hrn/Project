#include "thresholdsmanager.h"
#include <QDebug>

ThresholdsManager::ThresholdsManager() : settings("MyOrg", "CpuMonitor") {
    load();
}

SensorThreshold ThresholdsManager::getThreshold(const QString &sensor) const {
    // Создаем временную копию настроек для чтения
    QSettings tempSettings(settings.organizationName(), settings.applicationName());
    tempSettings.beginGroup("Thresholds");
    
    SensorThreshold threshold;
    threshold.min = tempSettings.value(sensor + "_min", 30).toInt();
    threshold.max = tempSettings.value(sensor + "_max", 70).toInt();
    
    tempSettings.endGroup();
    return threshold;
}

void ThresholdsManager::setThreshold(const QString &sensor, const SensorThreshold &threshold) {
    settings.beginGroup("Thresholds");
    settings.setValue(sensor + "_min", threshold.min);
    settings.setValue(sensor + "_max", threshold.max);
    settings.endGroup();
}

void ThresholdsManager::save() {
    // Автоматически сохраняется при изменении
}

void ThresholdsManager::load() {
    settings.beginGroup("Thresholds");
    QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        if (key.endsWith("_min")) {
            QString sensor = key.left(key.length() - 4);
            SensorThreshold threshold;
            threshold.min = settings.value(key).toInt();
            threshold.max = settings.value(sensor + "_max", 70).toInt();
            data[sensor] = threshold;
        }
    }
    settings.endGroup();
}
