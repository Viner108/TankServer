#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QPointF>
#include <QObject>

struct BulletInfo {
    QPointF position;
    double angle;
};

// Операторы сериализации для BulletInfo
QDataStream& operator<<(QDataStream& out, const BulletInfo& bullet);
QDataStream& operator>>(QDataStream& in, BulletInfo& bullet);

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readClient();

private:
    void calculateNewPosition(const QByteArray& data);
    QTcpSocket* clientSocket;
    QPointF tankPosition;
    int rotationAngle;
    int turretRotationAngle;
    QList<BulletInfo> bullets;
};

#endif // SERVER_H
