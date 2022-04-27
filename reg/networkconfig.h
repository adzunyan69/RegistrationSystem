#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QHostAddress>
#include <QDebug>
#include <QJsonObject>

namespace Reg {

#define make_shared_enabled(name)

class NetworkConfig
{
public:
    static std::shared_ptr<NetworkConfig> fromJson(const QJsonObject &json, QString &error);

    QHostAddress getAdress() const { return adress; } /*!< Метод, возвращающий IP-адрес системы */
    quint16 getPort() const { return port; } /*!< Метод, возвращающий порт системы */
    QAbstractSocket::SocketType getProtocol() const { return protocol; } /*!< Метод, возвращающий испоьзуемый проткоол */

    void write(QJsonObject &json_network_config);
    QString toString() const;

private:
    template<typename... T>
    inline static std::shared_ptr<NetworkConfig> create(T&&... t);

    NetworkConfig(const QHostAddress &adress,
                  const quint16 port,
                  const QAbstractSocket::SocketType protocol = QAbstractSocket::UdpSocket);

    const QHostAddress adress; /*!< IP-адрес потребителя */
    const quint16 port{ 0 }; /*!< Порт потребителя */
    const QAbstractSocket::SocketType protocol { QAbstractSocket::UnknownSocketType }; /*!< Tcp/Udp протокол */

    inline bool isValidAdressAndPort() const;
    inline bool isValidProtocol() const;

};

}; // namepsace Reg

#endif // NETWORKCONFIG_H
