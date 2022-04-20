#ifndef REGISTRATIONSYSTEM_H
#define REGISTRATIONSYSTEM_H

#include <QMainWindow>

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
};
#endif // REGISTRATIONSYSTEM_H
