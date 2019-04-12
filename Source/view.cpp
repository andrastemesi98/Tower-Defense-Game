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
    connect(_newGame, SIGNAL(clicked()), this, SLOT(runTest())); //a tesztet futtatja egyelőre
    connect(_settings, SIGNAL(clicked()), this, SLOT(settings()));
    connect(_exit, SIGNAL(clicked()), this, SLOT(exit()));
    menu->addWidget(_newGame);
    menu->addWidget(_settings);
    menu->addWidget(_exit);
    infos->addLayout(menu);
    setLayout(infos);


}

void print(model::game& g, std::vector<model::field*> *way = nullptr)
{
  if(way != nullptr)
    std::cout << "Dijkstra from (" << (*way)[0]->x << ","<< (*way)[0]->y << ") to ("
          << (*way).back()->x << "," << (*way).back()->y << ")\n";
  for(model::uint i = 0; i < g.sizey(); ++i)
    {
      for(model::uint j = 0; j < g.sizex(); ++j)
    {
      auto f = g.getField(j, i);
      if(f == nullptr)
        throw "HIBA VALAHOL? EZ NEM LEHET!\n";
      if(way != nullptr)
        {
          bool b = false;
          for(auto w : *way)
        if(w == f)
          {
            std::cout << "D ";
            b = true;
          }
          if(b) continue;
        }
      if(f->canBeEntered()) std::cout << "0 ";
      else std::cout << "1 ";
    }
      std::cout << "\n";
    }
  std::cout << std::flush;
}


template<class V>
void printpath(const V& v)
{
  std::cout << "path length = " << v.back()->dist << " => ";
  for(auto f : v)
    std::cout << "(" << f->x << "," <<  f->y << ") ";
  std::cout << std::endl;
}

void view::view::runTest()
{
    model::game g(5, 7);

    model::field* start = g.getField(2,1);
    model::field* goal = g.getField(3,6);
    std::vector<model::field*> fs;


    model::tower tt(nullptr, nullptr, 10, 10);
    //g.getField(6, 3)->addTower(&tt);
    //g.getField(5, 3)->addTower(&tt);
    g.getField(4, 3)->addTower(&tt);
    g.getField(3, 3)->addTower(&tt);
    g.getField(2, 3)->addTower(&tt);
    g.getField(1, 3)->addTower(&tt);
    print(g);

    bool b = model::dijkstra(fs, start, goal);
    if(!b)
      std::cerr << "Hibás dijkstra: nem talál utat\n";
    printpath(fs);
    print(g, &fs);
}

void view::view::settings()
{
    _infoLabel = new QLabel("Az 1. játékos (piros) az egér bal gombjával tornyot helyezhet le adott mezőre, a bázisára kattintva egységet indíthat onnan.\n\
A 2. játékos (zöld) a nyilakkal lépkedve kiválaszthatja a mezőt, melyre tornyot akar építeni, e körül szaggatott vonal jelenik meg.\n\
A ’t’ gomb megnyomására megépül a torony. Az ’e’ gomb megnyomására 1 egység indul a bázisából.\n\
Rövidítve emlékeztető:\n\
Első (piros) játékos: torony – bal egérgomb mezőre, egység – bal egérgomb bázisra.\n\
Második (zöld) játékos: mezőválasztás – nyilak, torony – ’t’, egység – ’e’.");
    infos->addWidget(_infoLabel, 0, Qt::AlignHCenter);
}

void view::view::exit()
{
    QMessageBox::information(this, "Kilépés", "A játékablak bezárul!");
    QApplication::quit();
}
