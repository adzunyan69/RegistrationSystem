#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>
#include <QSettings>

#ifdef CONFIG_DEBUG_PRINT
#include <QDebug>
#endif

namespace Common::Cfg {

template<typename EnumType>
class Config
{
public:
    Config() = delete;

    template<typename T>
    static T value(const EnumType& key, T&& defaultValue = T { })
    {
#ifdef CONFIG_DEBUG_PRINT
        qDebug().noquote().nospace() << QString("\n\treadValue(%1, %2)\n\tto file { %3 }\n\twith format { %4 }, status { %5 }")
                    .arg(static_cast<int>(key))
                    .arg(defaultValue)
                    .arg(settings.fileName())
                    .arg(settings.format())
                    .arg(settings.status());
#endif
        return qvariant_cast<T>(settings.value(keyToString[key], std::forward<T>(defaultValue)));
    }

    template<typename T>
    static void setValue(const EnumType& key,T&& value)
    {
#ifdef CONFIG_DEBUG_PRINT
        qDebug().noquote().nospace() << QString("\n\twriteValue(%1, %2)\n\tto file { %3 }\n\twith format { %4 }, status { %5 }")
                    .arg(static_cast<int>(key))
                    .arg(value)
                    .arg(settings.fileName())
                    .arg(settings.format())
                    .arg(settings.status());
#endif
        settings.setValue(keyToString[key], std::forward<T>(value));
        settings.sync();
    }

    static QSettings::Status status()
    {
        return settings.status();
    }

    // Clear all keys & values from config file
    static void clear()
    {
        settings.clear();
    }

    // Create config with custom values
    static void initConfig(const QMap<EnumType, QVariant>& keyToValues)
    {
        for(auto It = keyToValues.cbegin(); It != keyToValues.cend(); ++It)
        {
            settings.setValue(keyToString[It.key()], It.value());
        }
    }

    // Create config with empty values
    static void initEmptyValuesConfig()
    {
        for(auto It = keyToString.cbegin(); It != keyToString.cend(); ++It)
        {
            settings.setValue(It.value(), "");
        }
    }

private:
    static QSettings settings;
    static const QMap<EnumType, QString> keyToString;

};




}; // namespace Common

#endif // CONFIG_H
