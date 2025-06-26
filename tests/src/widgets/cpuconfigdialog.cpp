#include "cpuconfigdialog.h"
#include "cpureader.h"
#include "thresholdsmanager.h"

#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

CpuConfigDialog::CpuConfigDialog(ThresholdsManager *manager, QWidget *parent)
    : QDialog(parent),
      m_thresholdsManager(manager),
      m_sensorSelector(new QComboBox(this)),
      m_minSpin(new QSpinBox(this)),
      m_maxSpin(new QSpinBox(this)),
      m_saveButton(new QPushButton(tr("Сохранить"), this)),
      m_mainLayout(new QFormLayout(this))
{
    setupUI();
}

void CpuConfigDialog::setupUI()
{
    // Настройка диапазонов
    m_minSpin->setRange(0, 150);
    m_maxSpin->setRange(0, 150);
    
    // Настройка кнопки
    m_saveButton->setDefault(true);

    // Загрузка данных с датчиков
    loadSensors();

    // Добавление элементов в layout
    m_mainLayout->addRow(tr("Сенсор:"), m_sensorSelector);
    m_mainLayout->addRow(tr("Мин. температура (°C):"), m_minSpin);
    m_mainLayout->addRow(tr("Макс. температура (°C):"), m_maxSpin);
    m_mainLayout->addRow(m_saveButton);

    // Подключение сигналов
    connect(m_saveButton, &QPushButton::clicked, this, &CpuConfigDialog::onSave);
    connect(m_sensorSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CpuConfigDialog::onSensorChanged);

    setLayout(m_mainLayout);
    setWindowTitle(tr("Настройки температурных порогов"));
}

void CpuConfigDialog::loadSensors()
{
    m_sensorSelector->clear();
    
    try {
        QMap<QString, double> temps = CpuReader::readTemperatures();
        if (temps.isEmpty()) {
            m_sensorSelector->addItem(tr("Датчики не найдены"));
            m_sensorSelector->setEnabled(false);
            m_saveButton->setEnabled(false);
            return;
        }

        for (const QString &sensor : temps.keys()) {
            m_sensorSelector->addItem(sensor);
        }

        // Установка первого датчика как текущего
        if (m_sensorSelector->count() > 0) {
            QString firstSensor = m_sensorSelector->itemText(0);
            SensorThreshold current = m_thresholdsManager->getThreshold(firstSensor);
            m_minSpin->setValue(current.min);
            m_maxSpin->setValue(current.max);
        }
    } catch (const std::exception &e) {
        qCritical() << "Ошибка чтения датчиков:" << e.what();
        m_sensorSelector->addItem(tr("Ошибка загрузки датчиков"));
        m_sensorSelector->setEnabled(false);
        m_saveButton->setEnabled(false);
    }
}

void CpuConfigDialog::onSensorChanged(int index)
{
    if (index >= 0 && m_sensorSelector->isEnabled()) {
        QString sensor = m_sensorSelector->itemText(index);
        SensorThreshold current = m_thresholdsManager->getThreshold(sensor);
        m_minSpin->setValue(current.min);
        m_maxSpin->setValue(current.max);
    }
}

void CpuConfigDialog::onSave()
{
    if (m_sensorSelector->currentIndex() < 0 || !m_sensorSelector->isEnabled()) {
        QMessageBox::warning(this, tr("Ошибка"), 
                           tr("Нет доступных датчиков для сохранения"));
        return;
    }

    QString sensor = m_sensorSelector->currentText();
    SensorThreshold newThreshold(m_minSpin->value(), m_maxSpin->value());

    if (!newThreshold.isValid()) {
        QMessageBox::warning(this, tr("Ошибка"), 
                           tr("Минимальное значение должно быть меньше максимального"));
        return;
    }

    m_thresholdsManager->setThreshold(sensor, newThreshold);
    accept();
}
