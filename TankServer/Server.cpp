#include "Server.h"
#include <QDataStream>
#include <QtMath>
#include <QDebug>

QDataStream& operator<<(QDataStream& out, const BulletInfo& bullet) {
    out << bullet.position << bullet.angle;
    return out;
}

QDataStream& operator>>(QDataStream& in, BulletInfo& bullet) {
    in >> bullet.position >> bullet.angle;
    return in;
}

Server::Server(QObject* parent)
    : QTcpServer(parent), tankPosition(400, 300), rotationAngle(0), turretRotationAngle(0) {
}

void Server::incomingConnection(qintptr socketDescriptor) {
    clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readClient);
}

void Server::readClient() {
    QByteArray data = clientSocket->readAll();
    qDebug() << "Received data from client:" << data;
    calculateNewPosition(data);
}

void Server::calculateNewPosition(const QByteArray& data) {
    QDataStream in(data);
    int key;
    in >> key;

    qDebug() << "Key event received:" << key;

    switch (key) {
    case Qt::Key_Left:
        rotationAngle -= 15;
        if (rotationAngle < 0) rotationAngle += 360;
        break;
    case Qt::Key_Right:
        rotationAngle += 15;
        if (rotationAngle >= 360) rotationAngle -= 360;
        break;
    case Qt::Key_Up: {
        double radians = qDegreesToRadians(static_cast<double>(rotationAngle));
        double dx = 10 * qCos(radians);
        double dy = 10 * qSin(radians);
        tankPosition += QPointF(dx, dy);
        break;
    }
    case Qt::Key_Down: {
        double radians = qDegreesToRadians(static_cast<double>(rotationAngle));
        double dx = -10 * qCos(radians);
        double dy = -10 * qSin(radians);
        tankPosition += QPointF(dx, dy);
        break;
    }
    case Qt::Key_Q:
        turretRotationAngle -= 15;
        if (turretRotationAngle < 0) turretRotationAngle += 360;
        qDebug() << "Turret rotated left to" << turretRotationAngle;
        break;
    case Qt::Key_E:
        turretRotationAngle += 15;
        if (turretRotationAngle >= 360) turretRotationAngle -= 360;
        qDebug() << "Turret rotated right to" << turretRotationAngle;
        break;
    case Qt::Key_Space: {
        double radians = qDegreesToRadians(static_cast<double>(turretRotationAngle + rotationAngle));
        QPointF turretCenter = tankPosition + QPointF(25 * qCos(radians), 25 * qSin(radians));
        BulletInfo bullet = { turretCenter, static_cast<double>(turretRotationAngle + rotationAngle) };
        bullets.append(bullet);
        qDebug() << "Fire! Bullet position:" << bullet.position << "angle:" << bullet.angle;
        break;
    }
    }

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);
    out << tankPosition << rotationAngle << turretRotationAngle << bullets;
    clientSocket->write(outData);

    qDebug() << "Sent new position to client: position =" << tankPosition << "rotationAngle =" << rotationAngle << "turretRotationAngle =" << turretRotationAngle;
}
