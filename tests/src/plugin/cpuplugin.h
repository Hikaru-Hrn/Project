#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H

#include <QObject>
#include <lxqt/ilxqtpanelplugin.h>
#include "cpumonitorwidget.h"

class CpuPlugin : public QObject, public ILXQtPanelPlugin {
    Q_OBJECT

public:
    explicit CpuPlugin(const ILXQtPanelPluginStartupInfo &startupInfo);
    
    // Только объявления методов (без реализации)
    QWidget *widget() override;
    QString themeId() const override;
    void realign() override;
    QDialog *configureDialog() override;

private:
    CpuMonitorWidget m_widget;
};

class CpuPluginLibrary : public QObject, public ILXQtPanelPluginLibrary {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)

public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const override {
        return new CpuPlugin(startupInfo);
    }
};

#endif // CPUPLUGIN_H
