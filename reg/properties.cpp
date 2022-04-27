#include "properties.h"

namespace Reg {

Properties::Properties(QList<Property> &&properties)
    : properties(properties)
{ }

Property::Property(const QJsonObject &json)
    : name(json["name"].toString()),
      label(json["label"].toString()),
      selected_value_index{0}
{
    auto propertyValues = json["values"].toArray();
    for(auto value: propertyValues)
        values.append(PropertyValue(value.toObject()));

    setSelectedIndex(json["selected"].toInt(0));
}

PropertyValue::PropertyValue(const QJsonObject &json)
    : label(json["label"].toString()),
      value(static_cast<qint16>(json["value"].toInt())) { }

void Property::setSelectedIndex(int new_selected_value)
{
    if(new_selected_value < 0 || new_selected_value >= values.size())
        return;
    selected_value_index = new_selected_value;
}

void Property::selectedChanged(int new_selected_value)
{
    setSelectedIndex(new_selected_value);
}

std::shared_ptr<Properties> Properties::fromJson(const QJsonArray &json, QString &error)
{
    if(json.size() > 0) {
        QList<Property> newProperties;
        for(auto property: json) {
            newProperties.append(Property(property.toObject()));
        }

        return create(std::move(newProperties));
    } else {
        error = "Отсутствует перечень параметров в файле";
        qWarning() << error;
        return nullptr;
    }
}

template<typename... T>
std::shared_ptr<Properties> Properties::create(T&&... t)
{
    struct EnableMakeShared : public Properties {
        EnableMakeShared(T&&... args) : Properties(std::forward<T>(args)...) { }
    };

    return std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
}

void Properties::write(QJsonArray &json_properties)
{
    for(auto property: properties) {
        QJsonObject json_property;
        property.write(json_property);
        json_properties.append(json_property);
    }
}

void Property::write(QJsonObject &json_property)
{
    json_property["selected"] = selected_value_index;
    json_property["label"] = label;
    json_property["name"] = name;

    QJsonArray json_values;
    for(auto value: values) {
        QJsonObject json_property;
        value.write(json_property);
        json_values.append(json_property);
    }

    json_property["values"] = json_values;
}

PropertyValue Property::getSelectedValue() const
{
    return values.at(selected_value_index);
}

void PropertyValue::write(QJsonObject &json_property_value)
{
    json_property_value["label"] = label;
    json_property_value["value"] = value;
}

QString Properties::toString() const
{
    QString strProperties{"[Properties]:\n"};
    for(auto property: properties)
        strProperties += property.toString();

    return strProperties;
}

QString Property::toString() const

{
    QString strValues{"[Values]:\n"};
    for(auto value: values)
        strValues += value.toString();

    return QString("[Property]: name {%1}\t label {%2}\nselected: %3\n%4\n")
            .arg(name)
            .arg(label)
            .arg(QString::number(selected_value_index))
            .arg(strValues);
}

QString PropertyValue::toString() const
{
    return QString("[Value]: label {%1} \t value {%2}\n")
            .arg(label)
            .arg(QString::number(value));
}
}; // namespace Reg
