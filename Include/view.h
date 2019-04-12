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
    inline void setGameSize(model::uint x, model::uint y) {_sizex = x; _sizey = y;}

public slots:
    void runTest();
    void settings();
    void exit();

private:
    model::game* _game(model::uint x, model::uint y);
    QPushButton* _newGame;
    QPushButton* _settings;
    QPushButton* _exit;
    uint _sizex, _sizey; //lehet alapérték
    QHBoxLayout* menu; //3 kezdőgomb
    QVBoxLayout* infos;
    QLabel* _infoLabel;


};


}

#endif // VIEW_H
