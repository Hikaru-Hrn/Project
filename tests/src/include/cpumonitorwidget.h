#ifndef CPUMONITORWIDGET_H
#define CPUMONITORWIDGET_H

#include <QWidget>
#include <QMap>
#include <QProgressBar>
#include <QTimer>
#include <QVBoxLayout>
#include "thresholdsmanager.h"
#include "cpureader.h"

class CpuMonitorWidget : public QWidget {
    Q_OBJECT
public:
    explicit CpuMonitorWidget(QWidget *parent = nullptr);
    void realign();
    ThresholdsManager thresholds;

private slots:
    void updateTemperatures();

private:
    QMap<QString, QProgressBar*> bars;
    QTimer *timer;
    void setupUI();
    void updateProgressBarColor(const QString &sensor, double value);
};

#endif // CPUMONITORWIDGET_H
