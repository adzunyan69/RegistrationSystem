#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QHostAddress>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace Reg {

class Consumer;

class NetworkConfig
{
public:
    static std::shared_ptr<NetworkConfig> fromJson(const QJsonArray &json, QString &error);

    QList<Consumer> getConsumers() const { return consumers; }

    void write(QJsonArray &json_network_config);
    QString toString() const;

private:
    template<typename... T>
    inline static std::shared_ptr<NetworkConfig> create(T&&... t);

    NetworkConfig(QList<Consumer> &&consumers);

    const QList<Consumer> consumers;


};

class Consumer
{
public:
    Consumer(const QJsonObject &json_consumer, QString &error);

    QString getName() const { return name; }
    QString getLabel() const { return label; }
    QHostAddress getAdress() const { return adress; } /*!< Метод, возвращающий IP-адрес системы */
    quint16 getPort() const { return port; } /*!< Метод, возвращающий порт системы */
    quint16 getReceiverPort() const { return receiver_port; }
    QAbstractSocket::SocketType getProtocol() const { return protocol; } /*!< Метод, возвращающий испоьзуемый проткоол */

    void write(QJsonObject &json_consumer);
    QString toString() const;

private:
    QString name{ "consumer-undefined-name" };
    QString label{ "consumer-undefined-lable" };
    QHostAddress adress; /*!< IP-адрес потребителя */
    quint16 port{ 0 }; /*!< Порт потребителя */
    quint16 receiver_port{ 0 };
    QAbstractSocket::SocketType protocol { QAbstractSocket::UnknownSocketType }; /*!< Tcp/Udp протокол */

    inline bool isValid() const;
    inline bool isValidAdressAndPort() const;
    inline bool isValidProtocol() const;

};

}; // namepsace Reg

#endif // NETWORKCONFIG_H
