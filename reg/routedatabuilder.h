#ifndef ROUTEDATABUILDER_H
#define ROUTEDATABUILDER_H

#include <QObject>
#include <QDomDocument>
#include "reg/systemconfig.h"

class RouteDataBuilder : public QObject
{
    Q_OBJECT
public:
    explicit RouteDataBuilder(std::shared_ptr<Reg::SystemConfig> common_config,
                              std::shared_ptr<Reg::SystemConfig> system_config,
                              QObject *parent = nullptr)
        : QObject(parent),
          common_config(common_config),
          system_config(system_config){ }

    virtual QString toString() = 0;

protected:
    QDomDocument xml{};

    std::shared_ptr<Reg::SystemConfig> common_config{ nullptr };
    std::shared_ptr<Reg::SystemConfig> system_config{ nullptr };

};

class DefaultXMLRouteDataBuilder : public RouteDataBuilder
{
    Q_OBJECT
public:
    explicit DefaultXMLRouteDataBuilder(std::shared_ptr<Reg::SystemConfig> common_config,
                                        std::shared_ptr<Reg::SystemConfig> system_config,
                                 QObject *parent = nullptr)
        : RouteDataBuilder(common_config, system_config, parent) { }

    QString toString() override;

protected:
    virtual void appenSpecificData(QDomElement &root) { Q_UNUSED(root); }
    QDomElement createElementFromProperty(const Reg::Property& property);
    QDomElement createElement(const QString& tag, const QString& value);

private:
    void appendProperties(QDomElement &root);
    void appendRoute(QDomElement &root);

};

class PikXMLRouteDataBuilder : public DefaultXMLRouteDataBuilder
{
    Q_OBJECT
public:
    explicit PikXMLRouteDataBuilder(std::shared_ptr<Reg::SystemConfig> common_config,
                                    std::shared_ptr<Reg::SystemConfig> system_config,
                                    QObject *parent = nullptr)
        : DefaultXMLRouteDataBuilder(common_config, system_config, parent) { }

protected:
    void appenSpecificData(QDomElement &root) override;
};

#endif // ROUTEDATABUILDER_H
