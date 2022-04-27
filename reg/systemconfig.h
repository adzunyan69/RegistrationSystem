#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

#include <QJsonDocument>
#include "networkconfig.h"
#include "properties.h"
#include "../common/global.h"

namespace Reg {

class SystemConfig
{
public:
    static std::shared_ptr<SystemConfig> fromJson(const QString &filename);
    static std::shared_ptr<SystemConfig> fromJson(const QString &filename, QString &error);

    bool save();

    QString getLabel() const { return label; }
    QString getSystemName() const { return system_name; }
    bool isEnabled() const { return enabled; }
    auto& getNetworkConfig() { return network_config; }
    auto& getPropertiesConfig() { return properties; }
    auto& getProperties() { return properties->getProperties(); }

    QString toString();

private:
    void write(QJsonObject &json_system_config);
    static QJsonObject getJsonSystemConfig(const QString &filename, QString &error);

    template<typename... T>
    static std::shared_ptr<SystemConfig> create(T&&... t);

    SystemConfig(const QString &filename,
                 const bool enabled,
                 const QString &system_name,
                 const QString &label,
                 const std::shared_ptr<NetworkConfig> &network_config,
                 const std::shared_ptr<Properties> &properties);

    const QString filename{};
    bool enabled{ true };
    const QString system_name{ "undefined-system-name" };
    const QString label{ "undefined-system-label" };

    std::shared_ptr<NetworkConfig> network_config{ nullptr };
    std::shared_ptr<Properties> properties{ nullptr };
};

} // namespace Reg
#endif // SYSTEMCONFIG_H
