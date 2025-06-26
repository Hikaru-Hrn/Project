#include "cpureader.h"
#include <sensors/sensors.h>
#include <QRandomGenerator>
#include <QtMath>

QMap<QString, double> CpuReader::readTemperatures(bool useFakeData) {
    QMap<QString, double> temps;

    if (useFakeData) {
        // Симулируем температуры для двух фейковых сенсоров
        static double fakeTemp1 = 40.0; // Начальная температура для coretemp_0
        static double fakeTemp2 = 45.0; // Начальная температура для coretemp_1
        static bool increasing = true;

        // Функция для генерации случайного изменения температуры в диапазоне [0.5, 2.0]
        auto getRandomDelta = []() {
            return 0.5 + QRandomGenerator::global()->generateDouble() * 1.5;
        };

        // Имитация изменения температуры
        if (increasing) {
            fakeTemp1 += getRandomDelta(); // Увеличение на 0.5–2°C
            fakeTemp2 += getRandomDelta();
            if (fakeTemp1 > 90.0 || fakeTemp2 > 90.0) increasing = false; // Переключение на охлаждение
        } else {
            fakeTemp1 -= getRandomDelta(); // Уменьшение на 0.5–2°C
            fakeTemp2 -= getRandomDelta();
            if (fakeTemp1 < 30.0 || fakeTemp2 < 30.0) increasing = true; // Переключение на нагрев
        }

        temps["coretemp_0"] = fakeTemp1;
        temps["coretemp_1"] = fakeTemp2;
        return temps;
    }

    // Реальные данные через lm-sensors
    sensors_init(nullptr);
    sensors_chip_name const *chip;
    int chip_nr = 0;

    while ((chip = sensors_get_detected_chips(nullptr, &chip_nr))) {
        sensors_feature const *feature;
        int feature_nr = 0;
        while ((feature = sensors_get_features(chip, &feature_nr))) {
            if (feature->type == SENSORS_FEATURE_TEMP) {
                sensors_subfeature const *subfeature = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
                if (subfeature) {
                    double value;
                    if (sensors_get_value(chip, subfeature->number, &value) == 0) {
                        QString sensorName = QString("%1_%2").arg(chip->prefix).arg(feature->name);
                        temps[sensorName] = value;
                    }
                }
            }
        }
    }
    sensors_cleanup();

    return temps;
}
