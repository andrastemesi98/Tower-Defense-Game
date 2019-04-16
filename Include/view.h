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


}

#endif // VIEW_H
