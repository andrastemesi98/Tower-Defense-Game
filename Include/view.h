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
    inline void newGame(model::uint x, model::uint y) {_game(x, y);}
    //inline void setGameSize(model::uint x, model::uint y) {_sizex = x; _sizey = y;}

public slots:
    void run();
    void settings();
    void exit();
    void setSmallGame();
    void setMiddleGame();
    void setBigGame();
    void buttonClicked();

private:
    model::game* _game(model::uint x, model::uint y);
    //menü gombjai:
    QPushButton* _newGame;
    QPushButton* _settings;
    QPushButton* _exit;
    //pályaméret gombjai
    QPushButton* _smallGame;
    QPushButton* _middleGame;
    QPushButton* _bigGame;
    uint _sizex = 10, _sizey = 10; //pályaméret
    QHBoxLayout* menu; //3 kezdőgomb
    QHBoxLayout* sizeButtons; //pályaméret gombjai egymás mellett
    QVBoxLayout* infos; //dolgok egymás alatt
    QLabel* _infoLabel; //settingsben leírás, hogy hogyan kell játszani
    QLabel* _sizeLabel; //settingsben "Pályaméret:"

    QVector<QVector<QPushButton*>> buttonTable; //Új játékban a tábla
    QGridLayout* tableLayout;

    bool inNewgame = false, inSettings = false; //épp hol vagyunk

    void keyPressEvent(QKeyEvent *event);

};


}

#endif // VIEW_H
