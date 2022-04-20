#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <memory>

namespace Reg {

// надо сделать чтение сверху вниз, а не снизу-вверх, но проблемы с определением

class PropertyValue
{
public:
    PropertyValue(const QJsonObject &json);

    QString getLabel() const { return label; } /*!< Вернуть текст для лейбла */
    qint16 getValue() const { return value; } /*!< Вернуть код для этого значения */
    QString toString() const;
private:
    QString label{ "undefined-property_value-label" };
    qint16 value{ std::numeric_limits< decltype(value) >::min() };
};

class Property
{
public:
    using Values = QList<PropertyValue>;

    Property(const QJsonObject &json);

    void addValue(const PropertyValue &value) { values.append(value); } /*!< Добавить значение */
    Values getValues() const { return values; }  /*!< Вернуть rvalue список значений */
    QString getName() const { return name; }
    QString getLabel() const { return label; }
    QString toString() const;
private:
    QString name{ "undefined-property-name" };
    QString label{ "undefined-property-label" };
    Values values{ };

};

class Properties
{
public:
    static std::shared_ptr<Properties> fromJson(const QJsonArray &json, QString &error);

    QList<Property> getProperties() const { return properties; }

    QString toString() const;
private:
    template<typename... T>
    static std::shared_ptr<Properties> create(T&&... t)
    {
        struct EnableMakeShared : public Properties {
            EnableMakeShared(T&&... args) : Properties(std::forward<T>(args)...) { }
        };

        return std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    }

    Properties(QList<Property> &&properties) : properties(properties) { }

    const QList<Property> properties;
};




}; // namespace Reg

#endif // PROPERTIES_H
