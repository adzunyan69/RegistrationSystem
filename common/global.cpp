#include "global.h"

const QString Common::Global::system_params_dir{ QDir::currentPath() + "/cfg/system-widgets/"};

const QString Common::Global::SystemsParamsDir()
{
    if(QDir(system_params_dir).exists() == false)
        QDir().mkdir(system_params_dir);

    return system_params_dir;
}


/* Перечисление всех требуемых конфигов
 * Добавление нового конфига:
 *
 * enum с параметрами:
 * enum class EnumName { Param1, ... };
 *
 * Упрощение названия типа на основе шаблонного класса для настроек:
 * using ConfigNameType = RadAv::Config<EnumName>;
 *
 * Специализация шаблона с мапой параметр-название_в_файле
 * template<>
 * const QMap<EnumName, QString> ConfigName::keyToString {
 *  { EnumName::Param1, "params/param1" }
 * };
 *
 * Специализация шаблона для файла конфига
 * template<>
 * QSettings AppConfig::settings { "file.ini" };
 *
*/

/*! \brief
 * Мапа с параметр-название
 */
template<>
const QMap<Common::AppConfigEnum, QString> Common::AppConfig::keyToString {
    {AppConfigEnum::IP_RT, "network/ip_rt"}

};

/*! \brief
 * Путь к файлу с оновным конфигом
 */
template<>
QSettings Common::AppConfig::settings { "/cfg/config.ini" };
