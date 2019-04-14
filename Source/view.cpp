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
        //newGame(_sizex, _sizey);
    }
}

void view::view::settings()
{
    if(!inSettings)
    {
        inSettings = true;
        inNewgame = false;

        for (int i = 0; i < buttonTable.size(); i++)
        {
            for(int j = 0; j < buttonTable[0].size(); j++)
            {
                tableLayout->removeWidget(buttonTable[i][j]);
                delete buttonTable[i][j];
            }
        }

        //ha nem léteznek, akkor kell létrehozni őket:
        _infoLabel = new QLabel("Az 1. játékos (piros) az egér bal gombjával tornyot helyezhet le adott mezőre, a bázisára kattintva egységet indíthat onnan.\n\
    A 2. játékos (zöld) a nyilakkal lépkedve kiválaszthatja a mezőt, melyre tornyot akar építeni, e körül szaggatott vonal jelenik meg.\n\
    A ’t’ gomb megnyomására megépül a torony. Az ’e’ gomb megnyomására 1 egység indul a bázisából.\n\
    Rövidítve emlékeztető:\n\
    Első (piros) játékos: torony – bal egérgomb mezőre, egység – bal egérgomb bázisra.\n\
    Második (zöld) játékos: mezőválasztás – nyilak, torony – ’t’, egység – ’e’.");
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
}

void view::view::keyPressEvent(QKeyEvent *event)
{
    if(inNewgame)
    {
        if(event->key() == Qt::Key_T)
        {
            //...
        }
    }
}
