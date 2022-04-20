#include "systemconfig.h"

namespace Reg {

std::shared_ptr<SystemConfig> SystemConfig::fromJson(const QByteArray &jsonData)
{
    QString unusedError;
    return SystemConfig::fromJson(jsonData, unusedError);
}

std::shared_ptr<SystemConfig> SystemConfig::fromJson(const QByteArray &jsonData, QString &error)
{
    auto jsonError = std::make_shared<QJsonParseError>();
    auto json = QJsonDocument::fromJson(jsonData, jsonError.get())["system-config"].toObject();
    if(jsonError->error != QJsonParseError::NoError)
    {
        error = QString("Json Parse Error: %1 / offset: %2")
                .arg(jsonError->errorString())
                .arg(QString::number(jsonError->offset));
        qWarning() << error;
        return nullptr;
    }


    const bool enabled{ json["enabled"].toBool(true)};
    const QString system_name{ json["name"].toString("")};
    const QString label{ json["label"].toString("")};

    if(system_name.isEmpty() || label.isEmpty())
    {
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

    return create(enabled,
                  system_name,
                  label,
                  network_config,
                  properties);
}


SystemConfig::SystemConfig(const bool enabled,
                           const QString &system_name,
                           const QString &label,
                           const std::shared_ptr<NetworkConfig> &network_config,
                           const std::shared_ptr<Properties> &properties)
    : enabled(enabled),
      system_name(system_name),
      label(label),
      network_config(network_config),
      properties(properties)
{ }

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
