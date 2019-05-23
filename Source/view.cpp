#include "../Include/view.h"
#include <QMessageBox>

// NETWORK RÉSZ:

/* FOLYAMATA:
 * a multiplayer gomb megnyomásával kijön egy ablak, amiegy addresst kér el => ide csatlakozunk
 * ha most először játszunk egy multiplayert => kijön egy ablak, nickname-et kér
 * megnyílik a játék, AZONNAL kezdődik a játék.
 */

namespace view
{
// SLOTOK:

// ha sikertelen a csatlakozás => kilépünk/vissza a főmenübe (valami ilyesmi)
void view::conn_fail(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        MPDeleteScene();
        QMessageBox::critical(this, tr("Error"),
                              tr("The connection to the remote host has closed. "
                                 "Returning to the main menu."));
        MP_back_to_main_menu();
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Error"),
                              tr("The host was not found. Please check the "
                                 "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"),
                              tr("The connection was refused by the peer. "
                                 "Make sure the fortune server is running, "
                                 "and check that the host name and port "
                                 "settings are correct."));
        break;
    default:
        QMessageBox::critical(this, tr("Error"), tr("The following error occurred: %1.")
                              .arg(socket->errorString()));
    }
}

void view::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << "Read line:" << line;
        QTextStream str(&line);
        QString buf;
        str >> buf;
        if (buf == "NICKNAME")
        {
            str >> buf;
            MP_nickname_label->setText(QString("Ellenfél: ") + buf);
        }
        else if (buf == "ASK_TOWER")
        {
            uint px, py;
            str >> px; str >> py;
            bool success = _game->getPlayer(1).placeTower(0, _game->getField(px, py));
            if (! success) return;
            send_place_tower(px, py, 1);
            buttonTable[px][py]->setStyleSheet("background-color:LightGreen;");
        }
        // ha a modellel nem rendelkező játékos le akar rakni egy egységet:
        else if (buf == "ASK_UNIT")
        {
            // a modellel nem rendelkező játékos mindig az 1. játékos:
            _game->placeCreature(1);
            // a unit kirajzolásáért már nem felelős az ASK_UNIT üzenet.
        }
    }
}

// ha a server_socketből akarunk readelni a readyReadServer() fgv-en kívül,
// device not open errort kapunk a read-nél
void view::ReadLineFromServer(QString line)
{
    qDebug() << "Read line in non-model part:" << line;
    QTextStream str(&line);
    QString buf;
    str >> buf;
    if (buf == "NICKNAME")
    {
        str >> buf;
        MP_nickname_label->setText(QString("Ellenfél: ") + buf);
    }
    else if (buf == "GEOMETRY") // a játéktábla létrehozása:
    {
        str >> _sizex;
        str >> _sizey;
        // button table létrehozása:
        MPCreateButtonTable();
        // !! BASE-EK HELYÉNEK KISZÁMOLÁSA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else if (buf == "PLACE_TOWER")
    {
        uint px, py, pl;
        str >> px; str >> py; str >> pl;
        if (pl == 0) // ha a 0. játékos tornya =>
            buttonTable[px][py]->setStyleSheet("background-color:Red;");
        else
            buttonTable[px][py]->setStyleSheet("background-color:LightGreen;");
    }
    else if (buf == "UPDATE")
    {
        for (int i = 0; i < buttonTable.size(); i++)
            for(int j = 0; j < buttonTable[0].size(); j++)
                // ha korábban fekete vagy fehér volt =>
                if(buttonTable[i][j]->palette().background().color() == QColor(0,0,0) ||
                        buttonTable[i][j]->palette().background().color() == QColor(255,255,255))
                {
                    buttonTable[i][j]->setStyleSheet("background-color:orange;");
                    buttonTable[i][j]->setText("");
                }
    }
    else if (buf == "UNIT_INFO")
    {
        uint PlID, unitNum, posx, posy;
        str >> PlID; str >> unitNum; str >> posx; str >> posy;
        if (PlID == 0)
            buttonTable[posx][posy]->setStyleSheet("background-color:white;");
        if (PlID == 1)
            buttonTable[posx][posy]->setStyleSheet("background-color:black; color:white;");
        buttonTable[posx][posy]->setText(std::to_string(unitNum).c_str());
    }
    else if (buf == "WINNER")
    {
        int winner;
        str >> winner;
        QString winner_string = "";
        if(winner == 1) winner_string = nickname;
        else winner_string = MP_nickname_label->text().remove(0, 9);
        QMessageBox::information(this, "Játék vége", QString("A győztes játékos: ") + winner_string);
        MPDeleteScene();
        MP_back_to_main_menu();
    }
    else if (buf == "GOLD")
    {
        QString line2 = line.remove(0, 5);
        _golds->setText(line2);
    }
    else if (buf == "BASE_INFO")
    {
        uint posx, posy, owner, HP;
        str >> owner; str >> HP; str >> posx; str >> posy;
        switch(owner) {
        case 0 : buttonTable[posx][posy]->setStyleSheet("background-color:darkRed; color:white;"); break;
        case 1 : buttonTable[posx][posy]->setStyleSheet("background-color:darkGreen; color:white;"); break;
        }
        buttonTable[posx][posy]->setText(std::to_string(HP).c_str());
    }
}

// ------------------- ----------- ------------------- ---------------- ----------
// SERVERCLASS:
// ------------------- ----------- ------------------- ---------------- ----------

void serverclass::readyReadServer()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        _parent->ReadLineFromServer(line);
    }
}

void serverclass::disconnected()
{
    _parent->MPDeleteScene();
    QMessageBox::critical(_parent, "Hiba", "Megszakadt a kapcsolat a másik játékossal.");
    _parent->MP_back_to_main_menu();
    _parent->SLOT_wait_multiplayer_button();
}

void serverclass::incomingConnection(qintptr socketfd)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketfd);
    connect( socket, SIGNAL(readyRead()), this, SLOT(readyReadServer()) );
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    qDebug() << "Uj kapcsolat erkezett.\n";
    emit SIG_conn_established();
    //connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void serverclass::sendLine(QString line)
{
    socket->write(line.toStdString().c_str());
}

// ------------------- ----------- ------------------- ---------------- ----------
// SERVERCLASS VÉGE
// ------------------- ----------- ------------------- ---------------- ----------

void view::SLOT_wait_multiplayer_button()
{
    if (! wmp_pressed)
    {
        wait_multiplayer_button->setText("Stop Waiting for Multiplayer");
        wmp_pressed = true;
        _newGame->setEnabled(false);
        _settings->setEnabled(false);
        _exit->setEnabled(false);
        if (nickname == "")
        {
            bool ok;
            nickname = QInputDialog::getText(this, tr("Setting nickname..."), tr("Nickname:"),
                                             QLineEdit::Normal, "", &ok);
            if (!ok || nickname == "")
            {
                QMessageBox::information(this, "Hiba", "Nickname megadása kötelező.");
                SLOT_wait_multiplayer_button();
                return;
            }
        }
        start_multiplayer_button->setEnabled(false);
        // várakozó szerver felállítása:
        local_model = false; // ezt itt most garantáljuk.
        //server_socket = new QTcpSocket(this);
        server = new serverclass(this, server_socket);
        connect(server, SIGNAL(SIG_conn_established()), this, SLOT(SLOT_conn_established()) );
        qDebug() << "Created server...";
        bool success = server->listen(QHostAddress::Any, TOWER_DEF_PORT);
        if ( !success )
        {
            QMessageBox::information(this, "No Connection Found", "No connection has been found.");
            wait_multiplayer_button->setText("Sart Waiting for Multiplayer");
            wmp_pressed = false;
            _newGame->setEnabled(true);
            _settings->setEnabled(true);
            _exit->setEnabled(true);
            start_multiplayer_button->setEnabled(true);
            return;
        }
        // ha megvan a connection => meghívódik a megfelelő slot
    }
    else
    {
        wait_multiplayer_button->setText("Sart Waiting for Multiplayer");
        wmp_pressed = false;
        _newGame->setEnabled(true);
        _settings->setEnabled(true);
        _exit->setEnabled(true);
        start_multiplayer_button->setEnabled(true);
        // szerver megölése:
        server->close();
        delete server_socket;
        delete server;
        qDebug() << "Deleting server...";
    }
}

// ha megnyomtuk a "start multiplayer" gombot:
void view::SLOT_start_multiplayer_button()
{
    // address bekérése (mindig):
    bool ok;
    other_address = QInputDialog::getText(this, tr("Establishing connection..."),tr("Adress:"),
                                          QLineEdit::Normal,"localhost", &ok);
    // ha a cancelre nyomott rá => vissza a főmenübe, azaz: SEMMIT SE KELL TENNI.
    if (! ok) return;
    // nickname bekérése (ha még nincs megadva):
    if (nickname == "")
    {
        bool ok;
        nickname = QInputDialog::getText(this, tr("Setting nickname..."), tr("Nickname:"),
                                         QLineEdit::Normal, "", &ok);
        if (!ok || nickname == "")
        {
            QMessageBox::information(this, "Hiba", "Nickname megadása kötelező.");
            return;
        }
    }
    // csatlakozás a másik géphez (a bekért adatokkal):
    MP_connect();
}

// függvények:

void view::MP_connect()
{
    socket->connectToHost(other_address, TOWER_DEF_PORT);
    // ha a csatlakozás sikeres => meghívódik a connected() socket (automatikusan)
    // ha a csatlakozás sikertelen => meghívódik a conn_error() socket (automatikusan)
}

// ha sikeres a csatlakozás, BÁRMELYIK OLDALRÓL:
void view::MP_start_game()
{
    // A FŐMENÜ TARTALMÉNAK KITÖRLÉSE, új főmenü tartalom kirajzolása:
    main_stacked_widget->setCurrentWidget(MPMainWidget);
    MP_nickname_label = new QLabel;
    MPLayout->addWidget(MP_nickname_label);
    qDebug() << "MOOOOOST1";
    MP_nickname_label->setText("Ellenfél: enemy nickname");
    send_nickname();
    qDebug() << "MOOOOOST2";
    _golds = new QLabel("Arany: 1000");
    MPLayout->addWidget(_golds);
    qDebug() << "MOOOOOST3";
    //QMessageBox::information(this, tr("Nickname sent."),
    //                         tr(""));
    // ha nem itt van a model => nem teszünk semmit.
    if (! local_model) {
        //QMessageBox::information(this, tr("Not local model game starting...."),tr(""));
        return;
    }
    // ha itt van a local model:
    send_game_geometry(); // elküldjük a pályaméretet, és a bázisok elhelyezkedését
    MPCreateButtonTable();
    _game = new model::game(_sizex, _sizey); // <= model létrehozása
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(MPupdate()));
    _timer->start(50);
    timerExists = true;
    connect(_game, SIGNAL(goldChanged(QString)), this, SLOT(MPGoldChanged(QString)));
    connect(_game, SIGNAL(gameOver(int)), this, SLOT(MPGameOver(int)));
}

// ha sikeres a csatlakozás => megkezdjük a multiplayer játékot.
void view::connected()
{
    local_model = true; // aki sikeresen kezdeményezi a csatlakozást, ott lesz a local model
    MP_start_game();
}

// ha sikeres a RÁNK csatlakozás:
void view::SLOT_conn_established()
{
    local_model = false;
    MP_start_game();
}

void view::MP_back_to_main_menu()
{
    main_stacked_widget->setCurrentIndex(0); // vissza a főmenübe
    local_model = false;
}

// SENDEK, amiket mindkét gép küldhet:
void view::send_nickname()
{
    if(local_model)
        socket->write( (QString("NICKNAME ") + nickname+ QString("\n")).toUtf8() );
    else
        server->sendLine((QString("NICKNAME ") + nickname+ QString("\n")).toUtf8());
}

// SENDEK, amiket a modellel rendelkező gép küldhet => lényegében információ, hogy mit kell kirajzolni.

void view::send_game_geometry()
{
    socket->write( (QString("GEOMETRY ") + QString::number(_sizex) + QString(" ")
                    + QString::number(_sizey)+ QString("\n")).toUtf8() );
}

void view::send_place_tower(uint posx, uint posy, int playernum)
{
    socket->write( (QString("PLACE_TOWER ") + QString::number(posx) + QString(" ")
                    + QString::number(posy) + QString(" ") + QString::number(playernum)
                    + QString("\n")).toUtf8() );
}

void view::send_unit_position(uint posx, uint posy, uint PlID, uint unum)
{
    socket->write( (QString("UNIT_INFO ")+ QString::number(PlID) + QString(" ")
                    +QString::number(unum) + QString(" ")
                    +QString::number(posx) + QString(" ") + QString::number(posy)
                    + QString("\n")).toUtf8());
}

void view::send_base_info(uint posx, uint posy, uint owner, uint HP)
{
    socket->write( (QString("BASE_INFO ")+ QString::number(owner) + QString(" ")
                    +QString::number(HP) + QString(" ")
                    +QString::number(posx) + QString(" ") + QString::number(posy)
                    + QString("\n")).toUtf8());
}

// üzenetek, amiket a model nélküli játékos küldhet:
void view::send_ask_tower_placement(uint a, uint b)
{
    server->sendLine((QString("ASK_TOWER ") + QString::number(a) + QString(" ")
                      + QString::number(b) + QString("\n")).toUtf8());
}

void view::send_ask_unit_placement()
{
    server->sendLine(QString("ASK_UNIT\n"));
}

// MULTIPLAYER UI:

void view::MPno_model_buttonClicked()
{
    uint nr = tableLayout->indexOf(qobject_cast<QPushButton*>(sender()));
    uint a, b;
    a = nr / _sizex;
    b = nr % _sizex;
    send_ask_tower_placement(a, b);
}
void view::MPmodel_buttonClicked()
{
    uint nr = tableLayout->indexOf(qobject_cast<QPushButton*>(sender()));
    uint a, b;
    a = nr / _sizex;
    b = nr % _sizex;
    // a modellel rendellkező játékos mindig a 0. játékos!!
    bool success = _game->getPlayer(0).placeTower(0, _game->getField(a, b));
    if (! success) return;
    send_place_tower(a, b, 0);
    buttonTable[a][b]->setStyleSheet("background-color:Red;");
    // <= a 0. játékos lerakott egy tornyot
}

void view::MPGameOver(int winner)
{
    socket->write( (QString("WINNER ") + QString::number(winner)).toUtf8());
    QString winner_string = "";
    if(winner == 0) winner_string = nickname;
    else winner_string = MP_nickname_label->text().remove(0, 9);
    QMessageBox::information(this, "Játék vége", QString("A győztes játékos: ") + winner_string);
    MPDeleteScene();
    MP_back_to_main_menu();
}

void view::MPDeleteScene()
{
    if(local_model)
        delete _game;
    if(timerExists)
    {
        _timer->stop();
        timerExists = false;
    }
    inNewgame = false;
    _exit->setText("Kilépés");
    start_multiplayer_button->setVisible(true);
    wait_multiplayer_button->setVisible(true);
    MPLayout->removeWidget(_golds);
    delete _golds;
    MPLayout->removeWidget(MP_nickname_label);
    delete MP_nickname_label;
    for (int i = 0; i < buttonTable.size(); i++)
        for(int j = 0; j < buttonTable[0].size(); j++)
        {
            tableLayout->removeWidget(buttonTable[i][j]);
            delete buttonTable[i][j];
        }
    for (uint i = 0; i < _sizex; ++i)
        buttonTable[i].resize(0);
    buttonTable.resize(0);
    MPLayout->removeItem(tableLayout);
    return;
}

void view::MPGoldChanged(QString textt)
{
    _golds->setText(textt);
    //qDebug() << textt;
    socket->write((QString("GOLD ") + textt + QString("\n")).toUtf8());
}

// a timeout() hívja meg.
void view::MPupdate()
{
    socket->write("UPDATE\n");
    // <= jelzi, hogy az összes korábbi unit információt ki kell törölni.
    for (int i = 0; i < buttonTable.size(); i++)
        for(int j = 0; j < buttonTable[0].size(); j++)
        {
            // minden olyan mező alapra állítása, amin nincs akadály:
            if (_game->getField(i, j)->canBeEntered())
            {
                buttonTable[i][j]->setText("");
                buttonTable[i][j]->setStyleSheet("background-color:Orange;");
            }
            auto un = _game->getField(i,j)-> getUnit();
            if( un != nullptr) {
                switch(un->owner()->ID()) {
                case 0 : buttonTable[i][j]->setStyleSheet("background-color:white;"); break;
                case 1 : buttonTable[i][j]->setStyleSheet("background-color:black; color:white;"); break;
                }
                buttonTable[i][j]->setText(std::to_string(_game->getField(i,j)->unitNum()).c_str());
                send_unit_position(i, j, un->owner()->ID(), _game->getField(i,j)->unitNum());
            }
            model::base *bs = _game->getField(i,j)-> getBase();
            if( bs != nullptr) {
                switch(bs->owner()->ID()) {
                case 0 : buttonTable[i][j]->setStyleSheet("background-color:darkRed; color:white;"); break;
                case 1 : buttonTable[i][j]->setStyleSheet("background-color:darkGreen; color:white;"); break;
                }
                buttonTable[i][j]->setText(std::to_string(bs->HP()).c_str());
                send_base_info(i, j, bs->owner()->ID(), bs->HP());
            }
        }
}

void view::MPCreateButtonTable()
{
    tableLayout = new QGridLayout();
    buttonTable.resize(_sizex);
    for (uint i = 0; i < _sizex; ++i)
        buttonTable[i].resize(_sizey);

    for (int i = 0; i < buttonTable.size(); i++)
    {
        for(int j = 0; j < buttonTable[0].size(); j++)
        {
            buttonTable[i][j] = new QPushButton();
            buttonTable[i][j]->setFont(QFont("Times New Roman", 10, QFont::Bold));
            buttonTable[i][j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            tableLayout->addWidget(buttonTable[i][j], i, j);
            buttonTable[i][j]->setStyleSheet("background-color:orange;");
            if(local_model)
                connect(buttonTable[i][j], SIGNAL(clicked()), this, SLOT(MPmodel_buttonClicked()));
            else connect(buttonTable[i][j], SIGNAL(clicked()), this, SLOT(MPno_model_buttonClicked()));

        }
    }
    MPLayout->addLayout(tableLayout);
}


} // namespace view bezárása
//----------------------------------------------------------------------------------------
// NETWORK RÉSZ VÉGE
//----------------------------------------------------------------------------------------

/**
 * View class constructor
 *
 * Creates view object
 */
view::view::view(QWidget *parent) : QWidget(parent)
{
    resize(900, 900);
    setWindowTitle("Tower Defense");
    infos = new QVBoxLayout();
    menu = new QHBoxLayout();
    inNewgame = false;

    // stacked widget létrehozása:
    main_stacked_widget = new QStackedWidget;
    main_stacked_layout = new QHBoxLayout;

    main_normal_widget = new QWidget;
    MPMainWidget = new QWidget;
    MPLayout = new QVBoxLayout;

    _newGame = new QPushButton("Új Játék");
    _settings = new QPushButton("Beállítások");
    _exit = new QPushButton("Kilépés");
    start_multiplayer_button = new QPushButton("Multiplayer");
    wmp_pressed = false;
    wait_multiplayer_button = new QPushButton("Start Waiting for Multiplayer");
    connect(_newGame, SIGNAL(clicked()), this, SLOT(run()));
    connect(_settings, SIGNAL(clicked()), this, SLOT(settings()));
    connect(_exit, SIGNAL(clicked()), this, SLOT(exit()));
    connect(start_multiplayer_button, SIGNAL(clicked()), this, SLOT(SLOT_start_multiplayer_button()));
    connect(wait_multiplayer_button, SIGNAL(clicked()), this, SLOT(SLOT_wait_multiplayer_button()));
    menu->addWidget(_newGame);
    menu->addWidget(_settings);
    menu->addWidget(_exit);
    menu->addWidget(start_multiplayer_button);
    menu->addWidget(wait_multiplayer_button);
    infos->addLayout(menu);

    main_normal_widget->setLayout(infos);

    // multiplayer widgetek létrehozása:
    MPMainWidget->setLayout(MPLayout);
    //MP_nickname_label = new QLabel("Ellenfél: default nickname");
    //MPLayout->addWidget(MP_nickname_label);
    main_stacked_widget->addWidget(main_normal_widget);
    main_stacked_widget->addWidget(MPMainWidget);
    main_stacked_layout->addWidget(main_stacked_widget);
    this->setLayout(main_stacked_layout);
    main_stacked_widget->setCurrentIndex(0);
    // NETWORK RÉSZ:
    other_address = "";
    nickname = "";
    socket = new QTcpSocket(this);
    local_model = false;
    // <= ez majd akkor lesz "true", ha egy sikeres csatlakozást kezdeményeztünk a másik géphez.
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(conn_fail(QAbstractSocket::SocketError))); // ha sikertelen a csatlakozás
    server = nullptr; // A SZERVER ALAPBÓL NULLPTR, majd a wait for multiplayer állítja be
    server_socket = nullptr;
}

void view::view::run()
{
    _exit->setText("Főmenü");
    start_multiplayer_button->setVisible(false);
    wait_multiplayer_button->setVisible(false);
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
        _exit->setText("Főmenü");
        start_multiplayer_button->setVisible(false);
        wait_multiplayer_button->setVisible(false);
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

// a föggvény, amit a kilépés gomb meghív:
void view::view::exit()
{
    if(inSettings)
    {
        _exit->setText("Kilépés");
        start_multiplayer_button->setVisible(true);
        wait_multiplayer_button->setVisible(true);
        infos->removeItem(sizeButtons);
        sizeButtons->removeWidget(_sizeLabel);
        sizeButtons->removeWidget(_smallGame);
        sizeButtons->removeWidget(_middleGame);
        sizeButtons->removeWidget(_bigGame);
        delete _infoLabel;
        delete sizeButtons;
        delete _sizeLabel;
        delete _smallGame;
        delete _middleGame;
        delete _bigGame;
        inSettings = false;
        return;
    }
    if (inNewgame) // => vissza a főmenübe
    {
        delete _game;
        if(timerExists)
        {
            _timer->stop();
            timerExists = false;
            delete _golds;
        }
        inNewgame = false;
        _exit->setText("Kilépés");
        start_multiplayer_button->setVisible(true);
        wait_multiplayer_button->setVisible(true);
        infos->removeWidget(_golds);
        for (int i = 0; i < buttonTable.size(); i++)
        {
            for(int j = 0; j < buttonTable[0].size(); j++)
            {
                tableLayout->removeWidget(buttonTable[i][j]);
                delete buttonTable[i][j];
            }
        }
        for (uint i = 0; i < _sizex; ++i)
            buttonTable[i].resize(0);
        buttonTable.resize(0);
        infos->removeItem(tableLayout);
        return;
    }
    // idegesítő =>
    //QMessageBox::information(this, "Kilépés", "A játékablak bezárul!");
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
        return;
    }
    // <= ideáig volt a single playeres rész. INNEN -> MULTIPLAYERES rész:
    // amire reagál => a space lenyomásával rakunk le tornyot a megfelelő mezőre.
    if (event->key() == Qt::Key_E)
    {
        // a modellel rendelkező oldal lesz a 0. játékos:
        if (local_model == true)
            _game->placeCreature(0);
        else
            send_ask_unit_placement();
    }
}

void view::view::viewOver(int ID){
    _timer->stop();
    if(ID == 0) QMessageBox::information(this, "Vége", "Nyert a piros játékos!");
    if(ID == 1) QMessageBox::information(this, "Vége", "Nyert a zöld játékos!");
    settings();
}
