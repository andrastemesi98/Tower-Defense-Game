#ifndef SERVER_H
#define SERVER_H

#include "view.h" // beinkludáljuk a view-t, a könnyebb kezeléshez.
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <map>
#include <QSet>
#include <vector>
#include <QTextStream>

namespace view
{


class server : public QTcpServer
{
    Q_OBJECT
public:
    server(QObject* parent = nullptr);

private slots:
    void readyRead(); // Ha van mit olvasni valamelyik clienttől.
    void disconnected(); // Ha megszakadt a kapcsolat valamelyik clienttel.

protected:
    // öröklött fögvény a QTcpServer class-tól. A mi server-osztályunkból már nem lehet örökölni.
    virtual void incomingConnection(int socketfd) final;
private:
    std::vector<QString> messages;
    std::map<QTcpSocket *, QTcpSocket*> clients;
    // a map első tagjai clientek. Ha a második tag nem nullptr, akkor az első tag a
    // másodikkal van játékban éppen. Ha nullptr, akkor nincs játékban.
    std::map<int, std::pair<QTcpSocket*, QString> > ID_client_map;
    // a key: a client ID-je; a value: a client, akinek a key az ID-je, és a nickname-je
    // a client ID-t a server generálja => mindig az első szabad természetes számot kapja



    bool the_game_is_running(QTcpSocket*); // Ha az adott client játékban van
    void disconnect_client(QTcpSocket*, int id); // az adott clientet lecsatlakoztatja
    inline QTcpSocket* getClient(int ID) {try {return ID_client_map.at(ID).first;} catch(...){ return nullptr;}}
    inline QString getClientNickname(int ID) {try {return ID_client_map.at(ID).second;} catch(...){ return "";}}
};


}



#endif // SERVER_H
