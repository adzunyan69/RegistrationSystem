#include "registrationsystem.h"
#include "ui_registrationsystem.h"

RegistrationSystem::RegistrationSystem(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::RegistrationSystem)

{
    ui->setupUi(this);

    QGridLayout* grid_layout{ new QGridLayout(centralWidget()) };

    system_config_widget = new SystemConfigWidgetList(this);
    auto button{ new QPushButton("тыгыдык", this)};
    connect(button, &QPushButton::clicked,
            this, &RegistrationSystem::test);

    grid_layout->addWidget(system_config_widget);
    grid_layout->addWidget(button, 2, 0);
}

RegistrationSystem::~RegistrationSystem()
{
    delete ui;
}

void RegistrationSystem::test()
{
    auto cfg = system_config_widget->getSystemConfigs();
    for(auto config: cfg) {
        qInfo().noquote() << config->toString();
        if(config->getSystemName().toLower() == "pik") {
            PikXMLRouteDataBuilder dataBuilder(system_config_widget->getCommonConfig(),
                                               config);
            QString xmlPik = dataBuilder.toString();
            qInfo().noquote() << "XML_PIK: " << xmlPik;
            Sender *sender{ new Sender(config->getNetworkConfig()->getConsumers().first(), xmlPik) };
            Receiver *receiver{ new UdpReceiver(config->getNetworkConfig()->getConsumers().first(), this) };
        } else if(config->getSystemName().toLower() == "atape") {
            DefaultXMLRouteDataBuilder dataBuilder(system_config_widget->getCommonConfig(),
                                               config);
            qInfo().noquote() << "XML_ATAPE: " << dataBuilder.toString();
        }
    }



}

