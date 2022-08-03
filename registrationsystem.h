#ifndef REGISTRATIONSYSTEM_H
#define REGISTRATIONSYSTEM_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include "gui/system_config/systemconfigwidget.h"
#include "reg/routedatabuilder.h"
#include "reg/sender.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationSystem; }
QT_END_NAMESPACE

class RegistrationSystem : public QMainWindow
{
    Q_OBJECT

public:
    RegistrationSystem(QWidget *parent = nullptr);
    ~RegistrationSystem();

private:
    Ui::RegistrationSystem *ui;

    SystemConfigWidgetList* system_config_widget{ nullptr };

private slots:
    void test();
};
#endif // REGISTRATIONSYSTEM_H
