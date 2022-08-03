#include "sender.h"

Sender::Sender(Reg::Consumer consumer,
               const QString& msg,
               QObject *parent) : QObject(parent)
{
    if (consumer.getProtocol() == QAbstractSocket::UdpSocket) {
        sender = new QUdpSocket(this);
    } else if(consumer.getProtocol() == QAbstractSocket::TcpSocket) {
        sender = new QTcpSocket(this);
    }

    if (sender != nullptr) {
        QString ip = consumer.getAdress().toString();
        quint16 port = consumer.getPort();
        qDebug() << ip << "/" << port;
        sender->connectToHost(ip, port);
        sender->write(msg.toUtf8());
        isValid = true;
    }
}

UdpReceiver::UdpReceiver(Reg::Consumer consumer,
                         QObject *parent)
    : Receiver(parent)
{
    if (consumer.getReceiverPort() == 0 &&
        consumer.getProtocol() != QAbstractSocket::UdpSocket)
        return;

    qDebug() << "Udp Receiver " << consumer.getAdress() << " " << consumer.getReceiverPort();
    socket = new QUdpSocket(this);
    socket->bind(consumer.getReceiverPort());
    connect(socket, &QUdpSocket::readyRead, this, &UdpReceiver::read);

}

void UdpReceiver::read()
{
    QByteArray data;
    QDataStream in(&data, QIODevice::ReadOnly);
    do {
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size());
    } while(socket->hasPendingDatagrams());

    qDebug().noquote() << data;
}
