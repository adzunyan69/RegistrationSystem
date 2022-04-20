#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDir>
#include "config.h"

namespace Common {

/*!
 * \brief
 * Класс, представляющий интерфейс к доступы глобальных переменных, необходимых во всей программе
 * Определения вынесены в .cpp для того, чтобы не перекомпилировать все файлы
 */
class Global {
public:
    /*!
     * \brief
     * Метод, возвращающий путь к папке с JSON-файлами, которые содержат параметры системв формате /<dirs>/
     * \return QString в формате `/<dirs>/`
     */
    static const QString SystemsParamsDir();
private:
    /*!
     * \brief
     * Папка с JSON-файлами, которые содержат параметры систем
     */
    static const QString system_params_dir ;
};

/*!
 * \brief
 * Перечисление основных параметров приложения
 * \var AppConfigEnum::IP_RT
 * IP_RT var description temp
 */
enum class AppConfigEnum
{
    IP_RT
};

/*! \brief
 *  using-определние типа для основного конфига
 */
using AppConfig = Cfg::Config<AppConfigEnum>;

}; // namspace Common


#endif // GLOBAL_H
