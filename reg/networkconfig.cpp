#include "networkconfig.h"

namespace Reg {

NetworkConfig::NetworkConfig(QList<Consumer> &&consumers)
    : consumers(consumers)
{ }

std::shared_ptr<NetworkConfig> NetworkConfig::fromJson(const QJsonArray &json, QString &error)
{
    if(json.size() > 0) {
        QList<Consumer> newConsumers;
        for(auto consumer: json) {
            newConsumers.append(Consumer(consumer.toObject(), error));
        }

        return create(std::move(newConsumers));
    } else {
        error = "Отсусттвует перечень потребителей в файле";
        qWarning() << error;
        return nullptr;
    }


}

template<typename... T>
std::shared_ptr<NetworkConfig> NetworkConfig::create(T&&... t)
{
    struct EnableMakeShared : public NetworkConfig {
        EnableMakeShared(T&&... args) : NetworkConfig(std::forward<T>(args)...) { }
    };

    return std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
}

Consumer::Consumer(const QJsonObject &json, QString &error)
    : name(json["name"].toString()),
      label(json["label"].toString()),
      adress(json["ip"].toString()),
      port(json["port"].toInt()),
      receiver_port(json["receiver_port"].toInt())
{
    QString protocolString{ json["protocol"].toString().toLower() };
    protocol = protocolString == "udp" ?
                        QAbstractSocket::UdpSocket :
                        (protocolString == "tcp" ? QAbstractSocket::TcpSocket :
                                                   QAbstractSocket::UnknownSocketType);

    if(!isValid()) {
        error = QString("Неверные сетевые параметры: { adress: %1, port: %2, protocol: %3 / %4 }")
                .arg(adress.toString())
                .arg(QString::number(port))
                .arg(protocolString)
                .arg(QString::number(protocol));
    }
}

bool Consumer::isValid() const
{
    return isValidAdressAndPort() && isValidProtocol();
}

bool Consumer::isValidAdressAndPort() const
{
    return ((adress.protocol() ==  QAbstractSocket::IPv4Protocol ||
             adress.protocol() == QAbstractSocket::IPv6Protocol) &&
            port != 0);
}

bool Consumer::isValidProtocol() const
{
    return (protocol == QAbstractSocket::TcpSocket ||
            protocol == QAbstractSocket::UdpSocket);
}

void NetworkConfig::write(QJsonArray &json_network_config)
{
    for(auto consumer: consumers) {
        QJsonObject json_consumer;
        consumer.write(json_consumer);
        json_network_config.append(json_consumer);
    }
}

void Consumer::write(QJsonObject &json_consumer)
{
    json_consumer["name"] = name;
    json_consumer["label"] = label;
    json_consumer["ip"] = adress.toString();
    json_consumer["port"] = port;
    json_consumer["receiver_port"] = receiver_port;
    json_consumer["protocol"] = protocol == QAbstractSocket::UdpSocket ? "udp" :
                                                                         protocol == QAbstractSocket::TcpSocket ? "tcp":
                                                                                                                  "undefined";

}



QString NetworkConfig::toString() const
{
    QString strNetworkConfig{"[NetworkConfig]:\n"};
    for(auto consumer: consumers)
        strNetworkConfig += consumer.toString();

    return strNetworkConfig;

}

QString Consumer::toString() const
{
    return QString("[Consumer]: \nName: {1}\nLabel: {2}\nIP {%3}\nport {%4}\nprotocol {%5}\nreceiver_port: {%6}")
            .arg(name)
            .arg(label)
            .arg(adress.toString())
            .arg(QString::number(port))
            .arg(QString::number(protocol))
            .arg(QString::number(receiver_port));

}
}; // namspace Reg
