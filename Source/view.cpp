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

    if(!inNewgame)
    {
        inNewgame = true;
        _golds = new QLabel("1. játékos aranya: 1000 2. játékos aranya: 1000");
        _golds->setFixedSize(900, 30);
        infos->addWidget(_golds, 0, Qt::AlignHCenter);

        tableLayout = new QGridLayout();
        infos->addLayout(tableLayout);
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
        if(!timerExists)
        {
            _timer = new QTimer(this);
            connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
            _timer->start(50);
            timerExists = true;
        }

        newGame(_sizex, _sizey);
        //_game = new model::game(x, y);
        connect(_game, SIGNAL(goldChanged(QString)), _golds, SLOT(setText(QString)));
        connect(_game, SIGNAL(gameOver(int)), this, SLOT(viewOver(int)));
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
        }

        delete _golds;
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
    //_game->update();
    for (int i = 0; i < buttonTable.size(); i++)
    {
        for(int j = 0; j < buttonTable[0].size(); j++)
        {
            buttonTable[i][j]->setText("");
            buttonTable[i][j]->setStyleSheet("background-color:Orange;");
            model::base *bs = _game->getField(i,j)-> getBase();
            if( bs != nullptr) {
                switch(bs->owner()->ID()) {
                case 0 : buttonTable[i][j]->setStyleSheet("background-color:darkRed; color:white;"); break;
                case 1 : buttonTable[i][j]->setStyleSheet("background-color:darkGreen; color:white;"); break;
                }
              buttonTable[i][j]->setText(std::to_string(bs->HP()).c_str());
            }
            auto tw = _game->getField(i,j)-> getTower();
            if( tw != nullptr)
                switch(tw->owner()->ID()) {
                case 0 : buttonTable[i][j]->setStyleSheet("background-color:Red;"); break;
                case 1 : buttonTable[i][j]->setStyleSheet("background-color:LightGreen;"); break;
                }
            auto un = _game->getField(i,j)-> getUnit();
            if( un != nullptr) {
                switch(un->owner()->ID()) {
                case 0 : buttonTable[i][j]->setStyleSheet("background-color:white;"); break;
                case 1 : buttonTable[i][j]->setStyleSheet("background-color:black; color:white;"); break;
                }
                buttonTable[i][j]->setText(std::to_string(_game->getField(i,j)->unitNum()).c_str());
             }
        }
    }
    buttonTable[positionx][positiony]->setText("T");
    //std::string labelText="1. játékos aranya: " + std::to_string(_game->players()[0]->gold()) + " || 2. játékos aranya: " + std::to_string(_game->players()[1]->gold());
    //_golds->setText(labelText.c_str());
}

void view::view::setSmallGame()
{
    _sizex = 11;
    _sizey = 11;
    positionx = _sizex / 2;
    positiony = _sizey - 2;
}

void view::view::setMiddleGame()
{
    _sizex = 15;
    _sizey = 15;
    positionx = _sizex / 2;
    positiony = _sizey - 2;
}

void view::view::setBigGame()
{
    _sizex = 21;
    _sizey = 21;
    positionx = _sizex / 2;
    positiony = _sizey - 2;
}

void view::view::buttonClicked()
{
    uint nr = tableLayout->indexOf(qobject_cast<QPushButton*>(sender()));
    uint a, b;
    a = nr / _sizex;
    b = nr % _sizex;
    _game->getPlayer(0).placeTower(0, _game->getField(a, b));
}

void view::view::keyPressEvent(QKeyEvent *event)
{
    if(inNewgame)
    {
        if(event->key() == Qt::Key_0)
        {
            _game->placeCreature(0);
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
            _game->getPlayer(1).placeTower(0, _game->getField(positionx, positiony));
        }
        if(event->key() == Qt::Key_E)
        {
            _game->placeCreature(1);
        }
    }
}

void view::view::viewOver(int ID){
    _timer->stop();
    std::string message="Nyert a következő játékos:(0-bal/1-jobb) " + std::to_string(ID);
    QMessageBox::information(this, "Vége", message.c_str());
}
