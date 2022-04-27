#include "systemconfig.h"

namespace Reg {

SystemConfig::SystemConfig(const QString &filename,
                           const bool enabled,
                           const QString &system_name,
                           const QString &label,
                           const std::shared_ptr<NetworkConfig> &network_config,
                           const std::shared_ptr<Properties> &properties)
    : filename(filename),
      enabled(enabled),
      system_name(system_name),
      label(label),
      network_config(network_config),
      properties(properties)
{ }

std::shared_ptr<SystemConfig> SystemConfig::fromJson(const QString &filename)
{
    QString unusedError;
    return SystemConfig::fromJson(filename, unusedError);
}

std::shared_ptr<SystemConfig> SystemConfig::fromJson(const QString &filename, QString &error)
{
    auto json = getJsonSystemConfig(filename, error);
    if(error.isEmpty() == false)
        return nullptr;

    const bool enabled{ json["enabled"].toBool(true)};
    const QString system_name{ json["name"].toString("")};
    const QString label{ json["label"].toString("")};

    if(system_name.isEmpty() || label.isEmpty()) {
        error = QStringLiteral("Некорректные поля system_name или label в system-config");
        qWarning() << error;
        return nullptr;
    }

    const std::shared_ptr<NetworkConfig> network_config {
            NetworkConfig::fromJson(json["network-config"].toObject(), error) };
    const std::shared_ptr<Properties> properties {
            Properties::fromJson(json["properties"].toArray(), error) };

    if(network_config == nullptr || properties == nullptr)
        return nullptr;

    return create(filename,
                  enabled,
                  system_name,
                  label,
                  network_config,
                  properties);
}

QJsonObject SystemConfig::getJsonSystemConfig(const QString &filename, QString &error)
{
    QFile jsonFile{ Common::Global::SystemsParamsDir() + filename };

    if(jsonFile.open(QIODevice::ReadOnly) == false) {
        error = QString("Ошибка при открытии JSON-файла для чтения ( %1 )")
                .arg(jsonFile.fileName());
        qWarning() << error;
        return QJsonObject();
    }

    auto jsonError = std::make_shared<QJsonParseError>();

    auto json = QJsonDocument::fromJson(jsonFile.readAll(), jsonError.get());
    if(jsonError->error != QJsonParseError::NoError){
        error = QString("Ошибка при парсинге JSON-файла ( %1 / offset: %2 )")
                .arg(jsonError->errorString())
                .arg(QString::number(jsonError->offset));
        qWarning() << error;
        return QJsonObject();
    }

    return json["system-config"].toObject();
}
template<typename... T>
std::shared_ptr<SystemConfig> SystemConfig::create(T&&... t)
{
    struct EnableMakeShared : public SystemConfig {
        EnableMakeShared(T&&... args) : SystemConfig(std::forward<T>(args)...) { }
    };

    return std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
}

bool SystemConfig::save()
{
    QFile saveFile{ Common::Global::SystemsParamsDir() + filename };

    if(saveFile.open(QIODevice::WriteOnly) == false) {
        qWarning() << "Произошла ошибка при открытии JSON файла на запись";
        return false;
    }

    QJsonObject root;
    QJsonObject json_system_config;
    write(json_system_config);
    root["system-config"] = json_system_config;
    QJsonDocument saveDoc(root);
    saveFile.resize(0);
    saveFile.write(saveDoc.toJson());
    return true;
}

void SystemConfig::write(QJsonObject &json_system_config)
{
    json_system_config["enabled"] = enabled;
    json_system_config["label"] = label;
    json_system_config["name"] = system_name;

    QJsonObject json_network_config;
    network_config->write(json_network_config);
    json_system_config["network-config"] = json_network_config;

    QJsonArray json_properties;
    properties->write(json_properties);
    json_system_config["properties"] = json_properties;
}

QString SystemConfig::toString()
{
    return QString("[SystemConfig]: enabled: %1\t system_name: %2\t label: %3\n%4\n%5\n")
            .arg(QString::number(enabled))
            .arg(system_name)
            .arg(label)
            .arg(network_config->toString())
            .arg(properties->toString());
}

}
