#include "registrationsystem.h"
#include "ui_registrationsystem.h"

RegistrationSystem::RegistrationSystem(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::RegistrationSystem)

{
    ui->setupUi(this);

    QGridLayout* grid_layout{ new QGridLayout(centralWidget()) };

    system_config_widget = new SystemConfigWidget(Reg::SystemConfig::fromJson("correct_full.json"), this);
    auto button{ new QPushButton("тыгыдык", this)};
    connect(button, &QPushButton::clicked,
            this, &RegistrationSystem::test);

    grid_layout->addWidget(system_config_widget);
    grid_layout->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding), 1, 0);
    grid_layout->addWidget(button, 2, 0);

}

RegistrationSystem::~RegistrationSystem()
{
    delete ui;
}

void RegistrationSystem::test()
{
    auto cfg = system_config_widget->getSystemConfig();
    qInfo().noquote() << "new settings: " << cfg->toString();
}

