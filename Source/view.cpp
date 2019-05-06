#include "../Include/view.h"
#include <QMessageBox>
#include <QTextStream>

//--------------------------------------------------------------------------------
// Networking rész:
//--------------------------------------------------------------------------------
void view::view::onMultiplayerClicked() {
    // POPUP megjelenítése: "csatlakozás a szerverhez"
    socket->connectToHost(address, 4200); // BIZTOS HOGY 4200 ??? MI JÖN IDE?
}

// ha sikeres a csatlakozás a szerverre:
void view::view::connected() {
    // BELÉPNI A MULTIPLAYER MENÜ-VIEW-BA

    // LEKÉRNI A MÁR AKTÍV CLIENTEKET A SZERVERRŐL
}

void view::view::NETW_back_from_multiplayer_clicked()
{
    other_clients.resize(0);
    // a client nickname megmarad
    // üzenünk a szervernek, hogy lecsatlakozunk róla:
    socket->write((QString("CLIENT_LEAVE ") + QString::number(client_ID)).toUtf8());
    // socket->disconnect() // Disconnectelünk a socket-ről. =>  ez nem kell!!
    client_ID = -1;
    // BELÉPNI A FŐMENÜ VIEW-BA
}

void view::view::NETW_start_game_button_clicked(int index) {
    // ESETLEG: POPUP ABLAK, ÜZENETE: "Várakozás a másik játékos válaszára."
    // <= de ennek még nincs meg a struktúrája a szerverben
    // elküldeni a szerver-nek a "játékindítási szándékot":
    socket->write((QString("START_GAME") + QString::number(other_clients[index].first)).toUtf8());
    // az ID, amit küldünk: annak a clientnek az ID-ja, akire rákattintottunk
    // (azaz: at indexedik other_client ID-ja).
}

// ha olyan signalt kap, hogy van mit beolvasni a host-ról:
void view::view::readyRead() {
    while(socket->canReadLine())
    {
        // Beolvassuk a sorokat:
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        QTextStream str(&line);
        QString buf;
        str >> buf;
        if(buf == "FAIL_START_GAME") {
            // VISSZA A FŐMENÜBE
            // => ILYENKOR A MULTIPLAYER-ADATOKAT (kivéve nickname) TÖRÖLNI KELL!!
        }
        // ha sikeres volt a "játékindítási szándék:
        else if( buf == "SUCCESS_START_GAME") {
            // JÁTÉK VIEW MEGNYITÁSA
        }
        else if( buf == "MESSAGE_QUIT") {
            // POPUP ABLAK MEGNYITÁSA, üzenettel ("Megszakadt a kapcsolat a másik játékos oldalán")
            // VISSZA A FŐMENÜBE
        }
        // ha kiosztottak neki egy ID-t:
        else if ( buf == "ID") {
            // beolvassa a kiosztott ID-ját:
            str >> client_ID;
            // elküldi a nickname-jét
            socket->write((QString("NICKNAME ") + QString::number(client_ID) +
                           QString(" ") + client_nickname).toUtf8());
        }
        // ha van egy új játékos:
        else if ( buf == "NEW_CLIENT") {
            int clid;
            QString nickn;
            // új client ID-ját beolvassuk:
            str >> clid;
            if( clid == client_ID) // ha "mi" vagyunk az új client => semmit se csinálunk
                continue;
            // új client nickname-jét beolvassuk:
            str >> nickn;
            std::pair<int, QString> p(clid, nickn);
            // ellenőrizni, hogy nincs-e véletlenül azonos ID-jú client már a menüben:
            bool found = false;
            for(auto& p : other_clients)
                if(p.first == clid) {
                    found = true;
                    if(p.second != nickn)
                        break; // ha már valahogy hozzá volt adva korábban azonos nickname-el => minden ok
                    else p.second = nickn; // átírjuk a korábbi adatot az új nickname-re.
                }
            if(! found) // ha még nem volt ilyen ID-jú client:
            {
                other_clients.push_back(p);
                // A VIEW-T MEGVÁLTOZTATNI: JELENJEN MEG A MULTIPLAYER MENÜBEN AZ ÚJ SOR,
                // AMI AZ ÚJ CLIENTHEZ TARTOZIK !!!
            }
        }
        else if ( buf == "CLIENT_LEFT") {
            int clid;
            str >> clid;
            if(clid == client_ID) // ha mi magunk hagytuk el => semmit se csinálunk
                continue;
            int index = -1;
            for(unsigned int i = 0; i < other_clients.size(); ++i)
                if(other_clients[i].first == clid) {
                    index = i;
                    break;
                }
            // KIVENNI A VIEW-BÓL AZ index-EDIK SORT A MULTIPLAYER_MENÜ CLIENTJEI KÖZÜL
            other_clients.erase(other_clients.begin() + index); // kitöröljük az index-edik other clientet.
        }
        else {
            // POPUP MEGJELENIK, RAJTA: "ismeretlen üzenet a szervertől: " és a szerver üzenete.

        }
    }
}

//--------------------------------------------------------------------------------
// Networking rész vége.
//--------------------------------------------------------------------------------


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
    // A networkingért felelős rész:
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
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
            delete _golds;
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
        _infoLabel = new QLabel("Az 1. játékos (piros) az egér bal gombjával tornyot helyezhet le adott mezőre, a 'p'-t nyomva egységet indíthat.\n\
                                A 2. játékos (zöld) a 'WASD' gombokkal lépkedve kiválaszthatja a mezőt, melyre tornyot akar építeni, ezen 'T' jelenik meg.\n\
                                A ’t’ gomb megnyomására megépül a torony. Az ’e’ gomb megnyomására 1 egység indul a bázisából.\n\
                                                                                 Rövidítve emlékeztető:\n\
                                                                                                      Első (piros) játékos: torony – bal egérgomb mezőre, egység – 'p'.\n\
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
        if(event->key() == Qt::Key_P)
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
    if(ID == 0) QMessageBox::information(this, "Vége", "Nyert a piros játékos!");
    if(ID == 1) QMessageBox::information(this, "Vége", "Nyert a zöld játékos!");
    settings();
}
