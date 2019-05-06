#include "../Include/server.h"

namespace view
{

server::server(QObject *parent) : QTcpServer(parent)
{

}

void server::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients[client] = nullptr; // Az új csatlakozásnál biztos hogy nincs játékban a küldő client
    // Ha a clienttől tudunk üzenetet olvasni:
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    // Ha a clienttel megszakadt a kapcsolat:
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    // client ID generálása:
    int cid = 0;
    while(ID_client_map.count(cid) > 0) {++cid;}
    ID_client_map[cid] = std::pair<QTcpSocket*, QString>(client, "");
    // elküldi a client-nek az ID-ját
    client->write( (QString("ID ") + QString::number(cid)).toUtf8() );
}

bool server::the_game_is_running(QTcpSocket* cl)
{
    // Ha az adott client játékban van
    try {
        return clients.at(cl) != nullptr;
    }
    catch(...) {
        return false; // ha valamiért úgy hívjuk meg a fgv-t, hogy cl nincs is csatlakozva
        // => visszatérünk false-szal (nem kellene szerver szinten hibát dobni).
    }
}

void server::disconnect_client(QTcpSocket* cl, int id)
{
    try {
        // ha valakivel játékban volt:
        if(clients.at(cl) != nullptr) {
            clients.at(cl)->write(QString("MESSAGE_QUIT").toUtf8());
        }
    }
    catch(...) {} // ne dobjunk hibát
    clients.erase(cl); // kivesszük a clients map-ből
    if(id == -1)// ha nem tudta a hívó függvény az ID-t => be kell azonosítanunk:
        for(auto& p : ID_client_map)
            if(p.second.first == cl) {
                id = p.first;
                break; // megvan az id
            }
    ID_client_map.erase(id);
}

void server::readyRead()
{
    // a küldő client beazonosítása:
    QTcpSocket* cl = (QTcpSocket*)sender();

    // ha még nem indult el a játék => a clientek a főmenüben vannak.
    if( ! the_game_is_running(cl)) {
        // Ekkor a megengedett üzenetek:
        // "START_GAME ID" formában => ekkor a ekkor a küldő client új játékot kezd
        // "CLIENT_LEAVE ID" formában => ekkor a client kilépett a multiplayer menüből
        // "NICKNAME client_ID-ja client_nickname-je" => ekkor "készült el" teljesen a kapcsolat.
        QString line = QString::fromUtf8(cl->readLine()).trimmed();
        QTextStream str(&line);
        QString buf;
        str >> buf;
        if ( buf == "START_GAME") {
            str >> buf;
            int clID = buf.toInt(); // hogy melyik ID-jú client-hez csatlakozzunk
            auto other_cl = getClient(clID);
            if(other_cl == nullptr) {
                cl->write(QString("FAIL_START_GAME").toUtf8());
                return;
            }
            // beállítani a clientekben, hogy játékban vannak:
            clients[other_cl] = cl;
            clients[cl] = other_cl;
            cl->write(QString("SUCCESS_START_GAME").toUtf8());
            other_cl->write(QString("SUCCESS_START_GAME").toUtf8());
            // JÁTÉK ELKEZDÉSE: ITT
        }
        else if ( buf == "CLIENT_LEAVE") {
            int clid;
            str >> clid;
            // minden clientnek elküldeni, hogy kilépett ez a bizonyos client.
            for(auto& p : clients)
                p.first->write( (QString("CLIENT_LEFT ") + QString::number(clid)).toUtf8());
            disconnect_client(cl, clid);
        }
        else if (buf == "NICKNAME") {
            int clID;
            str >> clID;
            QString nickn;
            str >> nickn;
            try {
                // beállítjuk a client nickname-ét
                ID_client_map.at(clID).second = nickn;
                // elküldjük az összes "aktív" clientnek az új aktív client adatait,
                // annak is, aki maga az új aktív client (majd ő kiszűri, hogy fontos-e az infó)
                for(auto it = clients.begin(); it != clients.end(); ++it)
                    (*it).first->write( (QString("NEW_CLIENT ") + QString::number(clID) +
                                        QString(" ") + nickn).toUtf8());
            }
            catch(...) {}
        }
        return; // Ha a client nincs játékban => itt visszatérünk.
    }

    // ezen a ponton tudjuk, hogy a küldő játékban van.
    while(cl->canReadLine())
    {
        /* JÁTÉKBAN LEVŐ CLIENT ÜZENETEI:
         * TOWER x y    => új tornyot rak le x,y pontra
         * UNIT x y     => új egységet rak le x,y pontra
         * LESZ-E QUIT???
         */

    }
}

void server::disconnected()
{
    QTcpSocket* client = (QTcpSocket*)sender();
    disconnect_client(client, -1);
}
}
