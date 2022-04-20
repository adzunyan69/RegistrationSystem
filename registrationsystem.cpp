#include "registrationsystem.h"
#include "ui_registrationsystem.h"

RegistrationSystem::RegistrationSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegistrationSystem)
{
    ui->setupUi(this);

}

RegistrationSystem::~RegistrationSystem()
{
    delete ui;
}

