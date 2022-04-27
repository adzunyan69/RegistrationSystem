#include "systemconfigwidget.h"

SystemConfigWidget::SystemConfigWidget(std::shared_ptr<Reg::SystemConfig> system_config,
                                       QWidget *parent)
    : QWidget(parent),
      system_config(system_config),
      group_box(new QGroupBox(system_config->getLabel(), this)),
      enabled(new QCheckBox("Использовать", group_box))
{
    QLayout* layout { new QVBoxLayout(this) };
    layout->addWidget(group_box);
    network_config_widget = new NetworkConfigWidget(system_config->getNetworkConfig(), group_box);
    properties_widget = new PropertiesWidget(system_config->getPropertiesConfig(), group_box);

    QVBoxLayout* group_layout { new QVBoxLayout(group_box) };
    group_layout->addWidget(enabled);
    group_layout->addWidget(network_config_widget);
    group_layout->addWidget(properties_widget);

    connect(enabled, &QCheckBox::stateChanged,
            this, &SystemConfigWidget::changeEnabled);
    enabled->setChecked(system_config->isEnabled());
    changeEnabled();
}

void SystemConfigWidget::changeEnabled()
{
    network_config_widget->setEnabled(enabled->checkState());
    properties_widget->setEnabled(enabled->checkState());
}

std::shared_ptr<Reg::SystemConfig> SystemConfigWidget::getSystemConfig()
{
    if(properties_widget)
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
    group_layout->addWidget(createIpWidget());
    group_layout->addWidget(createPortWidget());
    group_layout->addWidget(createProtocolWidget());
}

QWidget* NetworkConfigWidget::createIpWidget()
{
    QWidget* ipWidget{ new QWidget(group_box) };
    QHBoxLayout* ipLayout{ new QHBoxLayout(ipWidget) };
    ipLayout->addWidget(new QLabel("IP: ", ipWidget));
    ipLayout->addWidget(new QLabel(network_config->getAdress().toString(), ipWidget));

    return ipWidget;
}

QWidget* NetworkConfigWidget::createPortWidget()
{
    QWidget* portWidget{ new QWidget(group_box) };
    QHBoxLayout* portLayout{ new QHBoxLayout(portWidget) };
    portLayout->addWidget(new QLabel("Порт: ", portWidget));
    portLayout->addWidget(new QLabel(QString::number(network_config->getPort()), portWidget));

    return portWidget;
}

QWidget* NetworkConfigWidget::createProtocolWidget()
{
    QWidget* protocolWidget{ new QWidget(group_box) };
    QHBoxLayout* protocolLayout{ new QHBoxLayout(protocolWidget) };
    auto protocol{ network_config->getProtocol() };
    protocolLayout->addWidget(new QLabel("Протокол: ", protocolWidget));
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
      label(new QLabel(property.getLabel(), this)),
      values(new QComboBox(this))
{
    QLayout* layout{ new QHBoxLayout(this) };
    layout->addWidget(label);
    layout->addWidget(values);

    for(auto &value: property.getValues())
        values->addItem(value.getLabel());

    values->setCurrentIndex(property.getSelectedIndex());
}
