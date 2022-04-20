#include "registrationsystem.h"

#include <QApplication>
#include "reg/systemconfig.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern("[%{time hh:mm:ss.zzz}] [%{file} %{function} line %{line}] [%{type}]: %{message}");

    RegistrationSystem w;
    w.show();
    return a.exec();
}


