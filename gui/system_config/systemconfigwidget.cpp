#include "systemconfigwidget.h"

SystemConfigWidgetList::SystemConfigWidgetList(QWidget *parent)
    : QWidget(parent)
{
    QLayout* layout{ new QVBoxLayout(this) };
    QScrollArea* scrollArea{ new QScrollArea(this) };
    layout->addWidget(scrollArea);
    QWidget* central{ new QWidget(this) };
    scrollArea->setAlignment(Qt::AlignCenter);
    QLayout* scrollAreaLayout{ new QVBoxLayout(central) };

    QDir jsonDir{ Common::Global::SystemsParamsDir() };
    for(auto json: jsonDir.entryList(QStringList("*.json"))) {
        systemConfigWidgets.append(new SystemConfigWidget(Reg::SystemConfig::fromJson(json), this));
        scrollAreaLayout->addWidget(systemConfigWidgets.last());
    }
    scrollArea->setWidget(central);

}

std::shared_ptr<Reg::SystemConfig> SystemConfigWidgetList::getCommonConfig() const
{
    for(auto systemConfigWidget: systemConfigWidgets) {
        if(systemConfigWidget->getSystemConfig()->getSystemName() == "common")
            return systemConfigWidget->getSystemConfig();
    }

    return  nullptr;
}

QList<std::shared_ptr<Reg::SystemConfig>> SystemConfigWidgetList::getSystemConfigs() const
{
    QList<std::shared_ptr<Reg::SystemConfig>> systemConfigList;
    for(auto systemConfigWidget: systemConfigWidgets) {
        if(systemConfigWidget->getSystemConfig()->getSystemName() != "common")
            systemConfigList.append(systemConfigWidget->getSystemConfig());
    }

    return systemConfigList;
}

SystemConfigWidget::SystemConfigWidget(std::shared_ptr<Reg::SystemConfig> system_config,
                                       QWidget *parent)
    : QWidget(parent),
      system_config(system_config),
      group_box(new QGroupBox(system_config->getLabel(), this)),
      enabled(new QCheckBox("Использовать", group_box))
{
    QLayout* layout { new QVBoxLayout(this) };
    layout->addWidget(group_box);
    properties_widget = new PropertiesWidget(system_config->getPropertiesConfig(), group_box);

    QVBoxLayout* group_layout { new QVBoxLayout(group_box) };
    group_layout->addWidget(enabled);
    if(system_config->getNetworkConfig() != nullptr) {
        network_config_widget = new NetworkConfigWidget(system_config->getNetworkConfig(), group_box);
        group_layout->addWidget(network_config_widget);
    }
    group_layout->addWidget(properties_widget);

    connect(enabled, &QCheckBox::stateChanged,
            this, &SystemConfigWidget::changeEnabled);
    enabled->setChecked(system_config->isEnabled());
    changeEnabled();
}

void SystemConfigWidget::changeEnabled()
{
    if(network_config_widget != nullptr)
        network_config_widget->setEnabled(enabled->checkState());
    if(properties_widget != nullptr)
        properties_widget->setEnabled(enabled->checkState());

    if(system_config != nullptr)
        system_config->setEnabled(enabled->checkState());
}

std::shared_ptr<Reg::SystemConfig> SystemConfigWidget::getSystemConfig()
{
    if(properties_widget != nullptr)
        properties_widget->updateDefaults();
    return system_config;
}

NetworkConfigWidget::NetworkConfigWidget(std::shared_ptr<Reg::NetworkConfig> network_config,
                                         QWidget *parent)
    : QWidget(parent),
      network_config(network_config),
      group_box(new QGroupBox("Сетевые параметры", this))
{
    QLayout* layout{ new QVBoxLayout(this) };
    layout->addWidget(group_box);

    QVBoxLayout* group_layout{ new QVBoxLayout(group_box) };
    for(auto consumer: network_config->getConsumers()) {
        QGroupBox *consumerGroupBox{ new QGroupBox(consumer.getName(), group_box) };
        QLayout *consumerGroupBoxLayout{ new QVBoxLayout(consumerGroupBox) };
        consumerGroupBoxLayout->addWidget(createIpWidget(consumer));
        consumerGroupBoxLayout->addWidget(createPortWidget(consumer));
        consumerGroupBoxLayout->addWidget(createProtocolWidget(consumer));

        group_layout->addWidget(consumerGroupBox);

    }
}

QWidget* NetworkConfigWidget::createIpWidget(const Reg::Consumer &consumer)
{
    QWidget* ipWidget{ new QWidget(group_box) };
    QHBoxLayout* ipLayout{ new QHBoxLayout(ipWidget) };
    ipLayout->addWidget(new ConfigLabel("IP: ", ipWidget));
    ipLayout->addWidget(new QLabel(consumer.getAdress().toString(), ipWidget));

    return ipWidget;
}

QWidget* NetworkConfigWidget::createPortWidget(const Reg::Consumer &consumer)
{
    QWidget* portWidget{ new QWidget(group_box) };
    QHBoxLayout* portLayout{ new QHBoxLayout(portWidget) };
    portLayout->addWidget(new ConfigLabel("Порт: ", portWidget));
    portLayout->addWidget(new QLabel(QString::number(consumer.getPort()), portWidget));

    return portWidget;
}

QWidget* NetworkConfigWidget::createProtocolWidget(const Reg::Consumer &consumer)
{
    QWidget* protocolWidget{ new QWidget(group_box) };
    QHBoxLayout* protocolLayout{ new QHBoxLayout(protocolWidget) };
    auto protocol{ consumer.getProtocol() };
    protocolLayout->addWidget(new ConfigLabel("Протокол: ", protocolWidget));
    protocolLayout->addWidget(new QLabel(
                               protocol == QAbstractSocket::TcpSocket ? "TCP" :
                              (protocol == QAbstractSocket::UdpSocket ? "UDP" : "Unknown Socket"),
                              protocolWidget)) ;

    return protocolWidget;
}

PropertiesWidget::PropertiesWidget(std::shared_ptr<Reg::Properties> properties,
                                   QWidget *parent)
    : QWidget(parent),
      properties(properties),
      group_box(new QGroupBox("Параметры", this))
{
    QLayout* layout{ new QVBoxLayout(this) };
    layout->addWidget(group_box);

    QLayout* group_layout{ new QVBoxLayout(group_box) };
    for(auto &property: properties->getProperties()) {
        property_widgets.append(new PropertyWidget(property, this));
        group_layout->addWidget(property_widgets.last());
    }
}

void PropertiesWidget::updateDefaults()
{
    if(property_widgets.size() != properties->getProperties().size()) {
        qWarning() << "Ошибка: размер свойств в GUI и в объекте не совпадает";
        return;
    }

    auto &properties_list = properties->getProperties();

    for(int i = 0; i < property_widgets.size(); ++i)
        properties_list[i].setSelectedIndex(property_widgets[i]->getSelectedValue());
}

PropertyWidget::PropertyWidget(const Reg::Property &property,
                               QWidget *parent)
    : QWidget(parent),
      label(new ConfigLabel(property.getLabel(), this)),
      values(new QComboBox(this))
{
    QLayout* layout{ new QHBoxLayout(this) };
    layout->addWidget(label);
    layout->addWidget(values);

    for(auto &value: property.getValues())
        values->addItem(value.getLabel());

    values->setCurrentIndex(property.getSelectedIndex());
}
