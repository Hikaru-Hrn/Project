#include "cpumonitorwidget.h"
#include <QProcess>
#include <QVBoxLayout>
#include <lxqt/ilxqtpanel.h>
#include <QTimer>
#include <QDebug>

CpuMonitorWidget::CpuMonitorWidget(QWidget *parent) : QWidget(parent) 
{
    setupUI();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CpuMonitorWidget::updateTemperatures);
    timer->start(thresholds.getThreshold("updateInterval").min * 1000);
}

void CpuMonitorWidget::setupUI() 
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QMap<QString, double> temps = CpuReader::readTemperatures(true);
    
    for (auto it = temps.constBegin(); it != temps.constEnd(); ++it) {
        QProgressBar *bar = new QProgressBar(this);
        bar->setOrientation(Qt::Vertical);
        bar->setMinimum(0);
        bar->setMaximum(100);
        bar->setValue(it.value());
        bar->setFormat(QString("%1: %2°C").arg(it.key()).arg(it.value(), 0, 'f', 1));
        bars.insert(it.key(), bar);
        layout->addWidget(bar);
    }
    setLayout(layout);
}

void CpuMonitorWidget::updateTemperatures() 
{
    QMap<QString, double> temps = CpuReader::readTemperatures(true);
    
    for (auto it = temps.constBegin(); it != temps.constEnd(); ++it) {
        if (bars.contains(it.key())) {
            bars[it.key()]->setValue(it.value());
            bars[it.key()]->setFormat(QString("%1: %2°C").arg(it.key()).arg(it.value(), 0, 'f', 1));
            updateProgressBarColor(it.key(), it.value());
        }
    }
}

void CpuMonitorWidget::updateProgressBarColor(const QString &sensor, double value) 
{
    SensorThreshold thresh = thresholds.getThreshold(sensor);
    QProgressBar *bar = bars[sensor];
    QString style;

    static QMap<QString, bool> notifiedHigh;
    static QMap<QString, bool> notifiedLow;

    if (value > thresh.max) {
        style = "QProgressBar::chunk { background-color: red; }";
        if (!notifiedHigh.value(sensor, false)) {
            QProcess::execute("notify-send", QStringList() 
                << QString("Высокая температура %1").arg(sensor)
                << QString("Температура %1°C превышает максимум %2°C").arg(value).arg(thresh.max));
            notifiedHigh[sensor] = true;
            notifiedLow[sensor] = false;
        }
    } 
    else if (value < thresh.min) {
        style = "QProgressBar::chunk { background-color: blue; }";
        if (!notifiedLow.value(sensor, false)) {
            QProcess::execute("notify-send", QStringList() 
                << QString("Низкая температура %1").arg(sensor)
                << QString("Температура %1°C ниже минимума %2°C").arg(value).arg(thresh.min));
            notifiedLow[sensor] = true;
            notifiedHigh[sensor] = false;
        }
    } 
    else {
        style = "QProgressBar::chunk { background-color: green; }";
        notifiedHigh[sensor] = false;
        notifiedLow[sensor] = false;
    }
    bar->setStyleSheet(style);
}

void CpuMonitorWidget::realign() 
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    if (!layout) {
        return;
    }

    // Получаем указатель на виджет панели
    QWidget *panelWidget = parentWidget();
    if (!panelWidget) {
        return;
    }

    // Определяем ориентацию по геометрии
    bool isHorizontal = panelWidget->width() > panelWidget->height();
    
    // Настраиваем направление layout
    layout->setDirection(isHorizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    
    // Настраиваем progress bars
    for (QProgressBar *bar : bars.values()) {
        bar->setOrientation(isHorizontal ? Qt::Horizontal : Qt::Vertical);
        bar->setFixedWidth(isHorizontal ? thresholds.getThreshold("tempBarWidth").min : QWIDGETSIZE_MAX);
        bar->setFixedHeight(isHorizontal ? QWIDGETSIZE_MAX : thresholds.getThreshold("tempBarWidth").min);
    }
}
