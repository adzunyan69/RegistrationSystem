#include "properties.h"

namespace Reg {

std::shared_ptr<Properties> Properties::fromJson(const QJsonArray &json, QString &error)
{
    if(json.size() > 0)
    {
        QList<Property> newProperties;
        for(auto property: json)
        {
            newProperties.append(Property(property.toObject()));
        }

        return create(std::move(newProperties));
    }
    else
    {
        error = "Отсутствует перечень параметров в файле";
        qWarning() << error;
        return nullptr;
    }
}

PropertyValue::PropertyValue(const QJsonObject &json)
    : label(json["label"].toString()),
      value(static_cast<qint16>(json["value"].toInt())) { }

Property::Property(const QJsonObject &json)
    : name(json["name"].toString()),
      label(json["label"].toString())
{
    auto propertyValues = json["values"].toArray();
    for(auto value: propertyValues)
    {
        values.append(PropertyValue(value.toObject()));
    }
}

QString PropertyValue::toString() const
{
    return QString("[Value]: label {%1} \t value {%2}\n")
            .arg(label)
            .arg(QString::number(value));
}

QString Property::toString() const

{
    QString strValues{"[Values]:\n"};
    for(auto value: values)
        strValues += value.toString();

    return QString("[Property]: name {%1}\t label {%2}\n%3\n")
            .arg(name)
            .arg(label)
            .arg(strValues);
}

QString Properties::toString() const
{
    QString strProperties{"[Properties]:\n"};
    for(auto property: properties)
        strProperties += property.toString();

    return strProperties;
}

}; // namespace Reg
