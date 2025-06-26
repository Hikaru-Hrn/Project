#ifndef CPUCONFIGDIALOG_H
#define CPUCONFIGDIALOG_H

#include <QDialog>
#include "thresholdsmanager.h"  // Теперь структура берется отсюда

// Предварительные объявления Qt классов
class QComboBox;
class QSpinBox;
class QPushButton;
class QFormLayout;

class CpuConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CpuConfigDialog(ThresholdsManager *manager, QWidget *parent = nullptr);
    ~CpuConfigDialog() override = default;

private slots:
    void onSave();
    void onSensorChanged(int index);

private:
    void setupUI();
    void loadSensors();

    ThresholdsManager *m_thresholdsManager;
    QComboBox *m_sensorSelector;
    QSpinBox *m_minSpin;
    QSpinBox *m_maxSpin;
    QPushButton *m_saveButton;
    QFormLayout *m_mainLayout;
};

#endif // CPUCONFIGDIALOG_H
