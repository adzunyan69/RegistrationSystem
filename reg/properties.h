#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <memory>
#include <utility>

namespace Reg {

class Property;
class PropertyValue;

class Properties
{
public:
    static std::shared_ptr<Properties> fromJson(const QJsonArray &json, QString &error);

    auto& getProperties() { return properties; }
    void write(QJsonArray &json_properties);

    QString toString() const;
private:
    template<typename... T>
    inline static std::shared_ptr<Properties> create(T&&... t);

    Properties(QList<Property> &&properties);
    QList<Property> properties;
};

class Property
{
public:
    using Values = QList<PropertyValue>;

    Property(const QJsonObject &json);

    int getSelectedIndex() const { return selected_value_index; }
    PropertyValue getSelectedValue() const;
    void setSelectedIndex(int new_selected_index);
    Values getValues() const { return values; }  /*!< Вернуть rvalue список значений */
    QString getName() const { return name; }
    QString getLabel() const { return label; }
    void write(QJsonObject &json_property);
    void addValue(const PropertyValue &value) { values.append(value); } /*!< Добавить значение */

    QString toString() const;
private:
    QString name{ "undefined-property-name" };
    QString label{ "undefined-property-label" };
    int selected_value_index{ 0 };
    Values values{ };

private slots:
    void selectedChanged(int new_selected_value);

};

class PropertyValue
{
public:
    PropertyValue(const QJsonObject &json_property_value);

    QString getLabel() const { return label; } /*!< Вернуть текст для лейбла */
    qint16 getValue() const { return value; } /*!< Вернуть код для этого значения */
    void write(QJsonObject &json);

    QString toString() const;
private:
    QString label{ "undefined-property_value-label" };
    qint16 value{ std::numeric_limits< decltype(value) >::min() };
};






}; // namespace Reg

#endif // PROPERTIES_H
