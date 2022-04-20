#include "networkconfig.h"

namespace Reg {

std::shared_ptr<NetworkConfig> NetworkConfig::fromJson(const QJsonObject &json, QString &error)
{
    const QHostAddress adress{ json["ip"].toString() };
    const quint16 port{ static_cast<quint16>(json["port"].toInt()) };
    QString protocolString{ json["protocol"].toString().toLower() };
    auto protocol = protocolString == "udp" ?
                        QAbstractSocket::UdpSocket :
                        (protocolString == "tcp" ? QAbstractSocket::TcpSocket :
                                                   QAbstractSocket::UnknownSocketType);

    auto networkConfig = create(adress, port, protocol);
    if(networkConfig->isValidAdressAndPort() &&
       networkConfig->isValidProtocol())
    {
        return networkConfig;
    }
    else
    {
        error = QString("Неверные сетевые параметры: { adress: %1, port: %2, protocol: %3 / %4 }")
                .arg(adress.toString())
                .arg(QString::number(port))
                .arg(protocolString)
                .arg(QString::number(protocol));

        return nullptr;
    }
}

NetworkConfig::NetworkConfig(const QHostAddress &adress,
                             const quint16 port,
                             const QAbstractSocket::SocketType protocol)
    : adress(adress),
      port(port),
      protocol(protocol)
{ }


bool NetworkConfig::isValidAdressAndPort() const
{
    return ((adress.protocol() ==  QAbstractSocket::IPv4Protocol ||
             adress.protocol() == QAbstractSocket::IPv6Protocol) &&
            port != 0);
}

bool NetworkConfig::isValidProtocol() const
{
    return (protocol == QAbstractSocket::TcpSocket ||
            protocol == QAbstractSocket::UdpSocket);
}

QString NetworkConfig::toString() const
{
    return QString("[Network Config]:\nIP {%1}\nport {%2}\nprotocol {%3}")
            .arg(adress.toString())
            .arg(QString::number(port))
            .arg(QString::number(protocol));
}
}; // namspace Reg
