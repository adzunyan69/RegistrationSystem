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
    static std::shared_ptr<SystemConfig> fromJson(const QByteArray &jsonData);
    static std::shared_ptr<SystemConfig> fromJson(const QByteArray &jsonData, QString &error);

    QString getLabel() const { return label; }
    QString getSystemName() const { return system_name; }
    bool isEnabled() const { return enabled; }
    auto getNetworkConfig() const { return network_config; }
    auto getProperties() const { return properties->getProperties(); }

    QString toString();

private:
    template<typename... T>
    static std::shared_ptr<SystemConfig> create(T&&... t)
    {
        struct EnableMakeShared : public SystemConfig {
            EnableMakeShared(T&&... args) : SystemConfig(std::forward<T>(args)...) { }
        };

        return std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    }

    SystemConfig(const bool enabled,
                 const QString &system_name,
                 const QString &label,
                 const std::shared_ptr<NetworkConfig> &network_config,
                 const std::shared_ptr<Properties> &properties);

    bool enabled{ true };
    const QString system_name{ "undefined-system-name" };
    const QString label{ "undefined-system-label" };

    const std::shared_ptr<NetworkConfig> network_config{ nullptr };
    const std::shared_ptr<Properties> properties{ nullptr };
};

} // namespace Reg
#endif // SYSTEMCONFIG_H
