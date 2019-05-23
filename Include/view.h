#pragma once
#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include "game.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QHBoxLayout>
#include <QApplication>
#include <QKeyEvent>
#include <QTimer>

// Networking (és ami még itt szükséges):
#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStackedWidget>

#define TOWER_DEF_PORT 15200
// Hogy melyik (rögzített) porton kömmunikálnak egymással a gépek.

namespace view
{
class game;
class view;
class serverclass;

class view : public QWidget
{
    Q_OBJECT
public:
    view(QWidget *parent = 0);
    //inline ~view() {delete _game(x, y);}
    //egyelőre a pályaméret állítható, ez kibővíthető játékmóddal
    inline void newGame(model::uint x, model::uint y) // ez csak a single player játékért felelős
    {_game = new model::game(x, y);}
    QTimer* _timer;

public slots:
    void run();
    void settings();
    void exit();
    void setSmallGame();
    void setMiddleGame();
    void setBigGame();
    void buttonClicked();
    void update();
    void viewOver(int ID);

    // ---------------------------------------------------------------------------------
    // NETWORK RÉSZ:

    void SLOT_conn_established(); // ha multiplayerre várunk, és valaki SIKERESEN ránk csatlakozott

    /* FOLYAMATA:
     * a multiplayer gomb megnyomásával kijön egy ablak, amiegy addresst kér el => ide csatlakozunk
     * ha most először játszunk egy multiplayert => kijön egy ablak, nickname-et kér
     * megnyílik a játék, AZONNAL kezdődik a játék.
     */
public:
    void ReadLineFromServer(QString line);
    // a server oldal ready-readje => ezt a serverclass hívja meg ha üzenetet kapott (soronként)
private slots:
    void SLOT_start_multiplayer_button();
    void SLOT_wait_multiplayer_button();
    void readyRead(); // Ha van mit beolvasni a socketből
    void connected(); /* Ha sikeres a csatlakozás a másik socketbe
                         => összekapcsolódott a két gép. */
    void conn_fail(QAbstractSocket::SocketError); // ha sikertelen a csatlakozás => vissza a főmenübe.
private:
    void MP_connect(); // megpróbálunk csatlakozni a másik géphez
    void MP_start_game(); // létrehozza a multiplayer játékhoz szükséges struktúrákat.
    // <= ez akkor hívódik meg, ha sikeres volt a csatlakokzás.
    void MP_back_to_main_menu(); // "visszatérés" a főmenübe.

    // üzenetek, amiket bármelyik oldal küldhet a másiknak:
    void send_nickname(); // elküldi a másiknak a választott nickname-et

    // üzenetek, amiket küldhetünk a másik gépnek, ha itt van a model:
    void send_game_geometry(); // elküldi a másik gépnek, hogy mekkora pályán játszunk
    // <= a bázisok helyét már a másik oldalon számoljuk ki
    void send_base_destroyed(int playernum);
    // <= elküldi a másik gépnek, ha megsemmisült egy base
    void send_place_tower(uint posx, uint posy, int playernum);
    // <= elküldi a másik gépnek, ha leraktunk egy tornyot.
    void send_remove_unit(uint posx, uint posy);
    // <= elküldi a másik gépnek, ha egy egység mozog.
    void send_place_unit(uint posx, uint posy, int playernum);

    // üzenetek, amiket küldhetünk a másik gépnek, ha a model a másik gépen van:
    // ??? <= van itt bármi is?

    // ------------------------------
    // Networking member variables:
    // főmenü:
    QPushButton* start_multiplayer_button;
    QPushButton* wait_multiplayer_button;

    // multiplayer játékrész:
    QWidget* MPMainWidget;
    QVBoxLayout* MPLayout;
    QLabel* MP_nickname_label;
    // A multiplayer gombokat majd disable-ölni kell a single player menüben.
    bool wmp_pressed;
    QTcpSocket* socket; // a model-t tartalmazó oldal használja, aki a kapcsolatot kezdeményezi
    QTcpSocket* server_socket; // a bemenő kapcsolatokért felelős socket (server ezt kapja paraméterként)
    serverclass* server; // a modelt nem tartalmazó, várakozó oldal használja
    QString other_address; // ez tartalmazza a másik gép címét
    QString nickname;
    // NETWORK RÉSZ VÉGE
    // ---------------------------------------------------------------------------------

    QHBoxLayout* main_stacked_layout;
    QStackedWidget* main_stacked_widget;
    QWidget* main_normal_widget;

    // MINDIG AZON A GÉPEN VAN A MODEL, AKI A KAPCSOLÓDÁST KEZDEMÉNYEZTE.
    model::game* _game;
    bool local_model;
    //menü gombjai:
    QPushButton* _newGame;
    QPushButton* _settings;
    QPushButton* _exit;
    //pályaméret gombjai
    QPushButton* _smallGame;
    QPushButton* _middleGame;
    QPushButton* _bigGame;
    uint _sizex = 11, _sizey = 11; //pályaméret
    QHBoxLayout* menu; //3 kezdőgomb
    QHBoxLayout* sizeButtons; //pályaméret gombjai egymás mellett
    QVBoxLayout* infos; //dolgok egymás alatt
    QLabel* _infoLabel; //settingsben leírás, hogy hogyan kell játszani
    QLabel* _sizeLabel; //settingsben "Pályaméret:"
    QLabel* _golds;

    QVector<QVector<QPushButton*>> buttonTable; //Új játékban a tábla
    QGridLayout* tableLayout;

    bool inNewgame = false, inSettings = false; //épp hol vagyunk
    bool timerExists = false;

    void keyPressEvent(QKeyEvent *event);
    uint positionx = _sizex / 2, positiony = _sizey - 2; //T a zöld bázis előtt

};

class serverclass : public QTcpServer
{
    Q_OBJECT
public:
    inline serverclass(view* parent, QTcpSocket* sc) : QTcpServer(parent), socket(sc), _parent(parent) {
        this->setMaxPendingConnections(1); // <= MAX 1 KAPCSOLAT LEHET AKTÍV
    }
    friend class view;
    void incomingConnection(qintptr handle);
    void sendLine(QString line);
private slots:
    //void readyReadServer();
    void readyReadServer(); // a server oldal ready-readje
    //void disconnected();
signals:
    void SIG_conn_established();
private:
    QTcpSocket* socket; // <= ez az a socket, ami a servert tartalmazó view része. => itt: client
    view* _parent;
};

}

#endif // VIEW_H
