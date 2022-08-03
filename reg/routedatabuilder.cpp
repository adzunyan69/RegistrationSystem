#include "routedatabuilder.h"

QString DefaultXMLRouteDataBuilder::toString()
{
    xml.appendChild( xml.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = xml.createElement("registrationStart");
    appendRoute(root);
    appendProperties(root);
    appenSpecificData(root);
    xml.appendChild(root);

    return xml.toString();
}

void DefaultXMLRouteDataBuilder::appendProperties(QDomElement &root)
{
    for(auto &property: system_config->getProperties())
        root.appendChild(createElementFromProperty(property));

    for(auto &property: common_config->getProperties())
        root.appendChild(createElementFromProperty(property));

}

void DefaultXMLRouteDataBuilder::appendRoute(QDomElement &root)
{
    root.appendChild(createElement("RegStart", "1"));
    root.appendChild(createElement("direction", "10901"));
    root.appendChild(createElement("putNumber", "1"));
    root.appendChild(createElement("putID", "110000134242"));
    root.appendChild(createElement("startCOORD_KM", "154"));
    root.appendChild(createElement("startCOORD_M", "305"));
    root.appendChild(createElement("seansFragID", "_debugFragID"));
    root.appendChild(createElement("seansID", "_debugID"));
}

QDomElement DefaultXMLRouteDataBuilder::createElementFromProperty(const Reg::Property &property)
{
    QDomElement element = xml.createElement(property.getName());
    auto selectedValue = property.getSelectedValue();
    element.appendChild(xml.createTextNode(QString::number(selectedValue.getValue())));
    element.setAttribute("label", selectedValue.getLabel());

    return element;
}

QDomElement DefaultXMLRouteDataBuilder::createElement(const QString &tag, const QString &value)
{
    QDomElement element = xml.createElement(tag);
    element.appendChild(xml.createTextNode(value));

    return element;
}

void PikXMLRouteDataBuilder::appenSpecificData(QDomElement &root)
{
    root.appendChild(createElement("rootBaseID", "2021-08-05 18:38:49.7560"));
    root.appendChild(createElement("dfxPath", "_debugDfx"));
    root.appendChild(createElement("dfxFilename", "debug_DfxFile"));
}
