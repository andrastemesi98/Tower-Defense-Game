#include "../Include/view.h"
#include <QMessageBox>


view::view::view(QWidget *parent) : QWidget(parent)
{
    resize(900, 900);
    setWindowTitle("Tower Defense");
    infos = new QVBoxLayout();
    menu = new QHBoxLayout();

    _newGame = new QPushButton("Új Játék");
    _settings = new QPushButton("Beállítások");
    _exit = new QPushButton("Kilépés");
    connect(_newGame, SIGNAL(clicked()), this, SLOT(run()));
    connect(_settings, SIGNAL(clicked()), this, SLOT(settings()));
    connect(_exit, SIGNAL(clicked()), this, SLOT(exit()));
    menu->addWidget(_newGame);
    menu->addWidget(_settings);
    menu->addWidget(_exit);
    infos->addLayout(menu);
    setLayout(infos);
}

void view::view::run()
{
    //ha léteznek, akkor kell kitörölni őket:
    if(inSettings)
    {
        delete _infoLabel;
        delete _sizeLabel;
        delete _smallGame;
        delete _middleGame;
        delete _bigGame;
        inSettings = false;
    }

    tableLayout = new QGridLayout();
    infos->addLayout(tableLayout);

    if(!inNewgame)
    {
        inNewgame = true;
        buttonTable.resize(_sizex);
        for (uint i = 0; i < _sizex; ++i)
        {
            buttonTable[i].resize(_sizey);
        }

        for (int i = 0; i < buttonTable.size(); i++)
        {
            for(int j = 0; j < buttonTable[0].size(); j++)
            {
                buttonTable[i][j] = new QPushButton();
                buttonTable[i][j]->setFont(QFont("Times New Roman", 10, QFont::Bold));
                buttonTable[i][j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
                tableLayout->addWidget(buttonTable[i][j], i, j);
                buttonTable[i][j]->setStyleSheet("background-color:orange;");

                connect(buttonTable[i][j], SIGNAL(clicked()), this, SLOT(buttonClicked()));
            }
        }
        newGame(_sizex, _sizey);

        if(!timerExists)
        {
            _timer = new QTimer(this);
            connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
            _timer->start(100);
            timerExists = true;
        }
    }
}

void view::view::settings()
{
    if(!inSettings)
    {
        inSettings = true;
        inNewgame = false;
        if(timerExists)
        {
            _timer->stop();
            timerExists = false;
            positionx = 0;
            positiony = 0;
        }

        for (int i = 0; i < buttonTable.size(); i++)
        {
            for(int j = 0; j < buttonTable[0].size(); j++)
            {
                tableLayout->removeWidget(buttonTable[i][j]);
                delete buttonTable[i][j];
            }
        }

        //ha nem léteznek, akkor kell létrehozni őket:
        _infoLabel = new QLabel("Az 1. játékos (piros) az egér bal gombjával tornyot helyezhet le adott mezőre, a '0'-t nyomva egységet indíthat.\n\
A 2. játékos (zöld) a 'WASD' gombokkal lépkedve kiválaszthatja a mezőt, melyre tornyot akar építeni, ezen 'T' jelenik meg.\n\
A ’t’ gomb megnyomására megépül a torony. Az ’e’ gomb megnyomására 1 egység indul a bázisából.\n\
Rövidítve emlékeztető:\n\
Első (piros) játékos: torony – bal egérgomb mezőre, egység – '0'.\n\
Második (zöld) játékos: mezőválasztás – 'WASD', torony – ’t’, egység – ’e’.");
        infos->addWidget(_infoLabel, 0, Qt::AlignHCenter);

        sizeButtons = new QHBoxLayout();
        _sizeLabel = new QLabel("Pályaméret:");
        _smallGame = new QPushButton("Kis pálya");
        _middleGame = new QPushButton("Közepes pálya");
        _bigGame = new QPushButton("Nagy pálya");
        connect(_smallGame, SIGNAL(clicked()), this, SLOT(setSmallGame()));
        connect(_middleGame, SIGNAL(clicked()), this, SLOT(setMiddleGame()));
        connect(_bigGame, SIGNAL(clicked()), this, SLOT(setBigGame()));

        sizeButtons->addWidget(_sizeLabel);
        sizeButtons->addWidget(_smallGame);
        sizeButtons->addWidget(_middleGame);
        sizeButtons->addWidget(_bigGame);
        infos->addLayout(sizeButtons);
    }
}

void view::view::exit()
{
    QMessageBox::information(this, "Kilépés", "A játékablak bezárul!");
    QApplication::quit();
}

void view::view::update()
{
    for (int i = 0; i < buttonTable.size(); i++)
    {
        for(int j = 0; j < buttonTable[0].size(); j++)
        {
            buttonTable[i][j]->setText("");
//ha 1. játékos bázisa, akkor buttonTable[i][j]->setStyleSheet("background-color:darkRed;");
//ha 1. játékos tornya, akkor buttonTable[i][j]->setStyleSheet("background-color:red;");
//ha 2. játékos bázisa, akkor buttonTable[i][j]->setStyleSheet("background-color:darkGreen;");
//ha 2. játékos tornya, akkor buttonTable[i][j]->setStyleSheet("background-color:lightGreen;");
//ha 1. játékos egysége, akkor buttonTable[i][j]->setStyleSheet("background-color:white;");
//ha 2. játékos egysége, akkor buttonTable[i][j]->setStyleSheet("background-color:black;");
//ha mindkettő egysége, akkor rá kép
        }
    }
    buttonTable[positionx][positiony]->setText("T");
}

void view::view::setSmallGame()
{
    _sizex = 10;
    _sizey = 10;
}

void view::view::setMiddleGame()
{
    _sizex = 20;
    _sizey = 20;
}

void view::view::setBigGame()
{
    _sizex = 30;
    _sizey = 30;
}

void view::view::buttonClicked()
{
    uint nr = tableLayout->indexOf(qobject_cast<QPushButton*>(sender()));
    uint a, b;
    a = nr / _sizex;
    b = nr % _sizex;
    //game-nek az a fv-e, ami 1. játékosnak (a,b)-re tornyot rak
}

void view::view::keyPressEvent(QKeyEvent *event)
{
    if(inNewgame)
    {
        if(event->key() == Qt::Key_0)
        {
            //game-nek az a fv-e, amelyik indít 1. játékosnak unitot
        }
        if(event->key() == Qt::Key_W)
        {
            if(positionx > 0) positionx--;
        }
        if(event->key() == Qt::Key_S)
        {
            if(positionx < _sizex - 1) positionx++;
        }
        if(event->key() == Qt::Key_A)
        {
            if(positiony > 0) positiony--;
        }
        if(event->key() == Qt::Key_D)
        {
            if(positiony < _sizey - 1) positiony++;
        }
        if(event->key() == Qt::Key_T)
        {
            //game-nek az a fv-e, ami 2. játékosnak (posx, posy)-ra tornyot rak
        }
        if(event->key() == Qt::Key_E)
        {
            //game-nek az a fv-e, amelyik indít 2. játékosnak unitot
        }
    }
}
