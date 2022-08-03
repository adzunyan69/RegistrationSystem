#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QAbstractSocket>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include "reg/networkconfig.h"

class Receiver;

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(Reg::Consumer consumer,
                    const QString& msg,
                    QObject *parent = nullptr);

private:
    QAbstractSocket *sender{ nullptr };

    Receiver *receiver{ nullptr };

    bool isValid{ false };

signals:
    void error();

};

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = nullptr) : QObject(parent) { }

};

class UdpReceiver : public Receiver
{
    Q_OBJECT
public:
    explicit UdpReceiver(Reg::Consumer consumer,
                         QObject *parent = nullptr);

private:
    QUdpSocket *socket{ nullptr };

private slots:
    void read();
};

#endif // SENDER_H
