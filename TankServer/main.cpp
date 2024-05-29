#include <QCoreApplication>
#include "Server.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    Server server;
    if (!server.listen(QHostAddress::Any, 1234)) { // Используйте нужный вам порт
        qCritical() << "Unable to start the server:" << server.errorString();
        return 1;
    }

    qInfo() << "Server started on port" << server.serverPort();
    return a.exec();
}
