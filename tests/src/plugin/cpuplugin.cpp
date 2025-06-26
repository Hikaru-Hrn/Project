#include "cpuplugin.h"
#include "cpuconfigdialog.h"

CpuPlugin::CpuPlugin(const ILXQtPanelPluginStartupInfo &startupInfo)
    : QObject(),
      ILXQtPanelPlugin(startupInfo),
      m_widget()
{
    // Конструктор
}

QWidget* CpuPlugin::widget()
{
    return &m_widget;
}

QString CpuPlugin::themeId() const
{
    return QStringLiteral("CpuSensorsPlugin");
}

void CpuPlugin::realign()
{
    m_widget.realign();
}

QDialog* CpuPlugin::configureDialog()
{
    return new CpuConfigDialog(&m_widget.thresholds, nullptr);
}
