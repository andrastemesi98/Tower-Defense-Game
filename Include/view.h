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

// Networking:
#include <QTcpSocket>
#include <QString>

namespace view
{
class game;
class view : public QWidget
{
    Q_OBJECT
public:
    view(QWidget *parent = 0);
    //inline ~view() {delete _game(x, y);}
    //egyelőre a pályaméret állítható, ez kibővíthető játékmóddal
    inline void newGame(model::uint x, model::uint y)
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

private:
    model::game* _game;
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
    QHBoxLayout* modeButtons; //játékmód gombjai egymás mellett
    QVBoxLayout* infos; //dolgok egymás alatt
    QLabel* _infoLabel; //settingsben leírás, hogy hogyan kell játszani
    QLabel* _sizeLabel; //settingsben "Pályaméret:"
    QLabel* _modeLabel; //settingsben "Játékmód:"
    QVBoxLayout* mode; //csatlakozott emberek egymás alatt
    QVector<QPushButton*> _connectedClients; //előző vektora
    QPushButton* _localGame;
    QPushButton* _networkGame;
    QLabel* _golds;

    QVector<QVector<QPushButton*>> buttonTable; //Új játékban a tábla
    QGridLayout* tableLayout;

    bool inNewgame = false, inSettings = false; //épp hol vagyunk
    bool timerExists = false;

    void keyPressEvent(QKeyEvent *event);
    uint positionx = _sizex / 2, positiony = _sizey - 2; //T a zöld bázis előtt

    //-------------------------------------------------------------------
    // Network rész:

    // ez nem így lesz:
    //signals:
    // A kommunikáció két (fő) lehetséges oka: tower/unit hozzáadása
    //void NETW_added_tower();
    //void NETW_added_unit();
    //void NETW_quit(); // Ha kilép a játékból. => valszeg ilyen nem lesz
private slots:
    void onMultiplayerClicked(); // Ha a főmenüben megnyomták a multiplayer gombot.
    void readyRead(); // Ha van mit beolvasni a serverről.
    void connected(); // Ha sikeres a csatlakozás a serverre => bekerülünk a multiplayer menübe.
    void NETW_start_game_button_clicked();
    // ha megnyomták az index-edik multiplayer menüben lévő játékos start game gombját,
    // amit lekér a függvényen belül => beindul a játék az adott játékossal.
    void NETW_back_from_multiplayer_clicked();
    // ha megnyomtuk a "vissza a főmenübe" gombot a multiplayer ablakban.
    // => lecsatlakozunk a szerverről, töröljük az itt tárolt adatokat.

private:
    // A Networkingért felelős rész:
    QTcpSocket* socket;
    QString address; // a server addresse => ITT NEM TUDOM MI VAN
    QString client_nickname = "default_nickname"; // az adott view-hoz tartozó client ID
    // Amikor először lépünk majd be a "multiplayer" menübe, meg kell adni.
    int client_ID = -1; // a mi saját client ID-nk.
    /* Folyamat =>
     * Client csatlakozik a szerverre (connectToHost()-tal)
     * Szerver visszaküldi a clientnek a kiosztott ID-ját (write ("ID ..." )-tal)
     * Client elküldi a szervernek a nickname-ét (write ("NICKANEME ...")-tal)
     * Szerver elküldi az összes clientnek az összes aktív client ID,nickname párját (write("NEW_CLIENT...")-tal)
     */

    // a multiplayer menüben a többi játékos, akik bent vannak:
    // a pair-ben int,QString párok: másik játékos ID-ja, és nickname-je.
    // azért rendezzük vectorba, hogy majd ki lehessen írni sorrendben őket
    std::vector< std::pair< int, QString> > other_clients;
    // !! Ez alapján kell majd beazonosítani a játékost, akivel új játékot kezdünk.
    //------------------------------------------------------------------

};


}

#endif // VIEW_H
