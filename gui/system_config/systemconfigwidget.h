#ifndef SYSTEMCONFIGWIDGET_H
#define SYSTEMCONFIGWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <reg/systemconfig.h>

class SystemConfigWidget;
class NetworkConfigWidget;
class PropertiesWidget;
class PropertyWidget;

class SystemConfigWidgetList : public QWidget
{
    Q_OBJECT
public:
    explicit SystemConfigWidgetList(QWidget *parent = nullptr);

    QList<std::shared_ptr<Reg::SystemConfig>> getSystemConfigs();
private:
    QList<SystemConfigWidget*> systemConfigWidgets;

};

class SystemConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SystemConfigWidget(std::shared_ptr<Reg::SystemConfig> system_config,
                                QWidget *parent = nullptr);

    std::shared_ptr<Reg::SystemConfig> getSystemConfig();

private:
    std::shared_ptr<Reg::SystemConfig> system_config{ nullptr };

    QGroupBox* group_box{ nullptr };
    QCheckBox* enabled{ nullptr};

    NetworkConfigWidget* network_config_widget{ nullptr };
    PropertiesWidget* properties_widget{ nullptr };

private slots:
    void changeEnabled();
};

class NetworkConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkConfigWidget(std::shared_ptr<Reg::NetworkConfig> network_config,
                                 QWidget *parent = nullptr);

private:
    std::shared_ptr<Reg::NetworkConfig> network_config{ nullptr };

    QGroupBox* group_box{ nullptr };

    QWidget* createIpWidget();
    QWidget* createPortWidget();
    QWidget* createProtocolWidget();
};

class PropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertiesWidget(std::shared_ptr<Reg::Properties> properties,
                              QWidget *parent = nullptr);

    void updateDefaults();

private:
    std::shared_ptr<Reg::Properties> properties{ nullptr };

    QGroupBox* group_box{ nullptr };
    QList<PropertyWidget*> property_widgets;
};

class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyWidget(const Reg::Property &property,
                            QWidget *parent = nullptr);

    int getSelectedValue() { return values->currentIndex(); }
private:
    QLabel* label{ nullptr };
    QComboBox* values{ nullptr };



};

class ConfigLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ConfigLabel(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent)
    {
        this->setWordWrap(true);
        this->setFixedWidth(80);
    }
};

#endif // SYSTEMCONFIGWIDGET_H
