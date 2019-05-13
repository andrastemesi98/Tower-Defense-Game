#include "../Include/view.h"
#include <iostream>

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    view::view w;
    w.show();
    return a.exec();*/
    model::game* _game;
    _game = new model::game(10, 10);

    //A bázisok létrejönnek, és élnek
    std::cout<<(_game->getField(5, 0)->getBase()->location() == _game->getField(5, 0))<<std::endl;
    std::cout<<(_game->getField(5, 9)->getBase()->location() == _game->getField(5, 9))<<std::endl;
    std::cout<<(_game->getField(5, 0)->getBase()->HP() > 0)<<std::endl;
    std::cout<<(_game->getField(5, 9)->getBase()->HP() > 0)<<std::endl;

    //A bázisokra nem lehet építeni
    std::cout<<(_game->getField(5, 0)->canBeEntered() == 0)<<std::endl;
    std::cout<<(_game->getField(5, 9)->canBeEntered() == 0)<<std::endl;

    //Eredetileg nincsenek tornyok, a nem bázis mezőkre nem lehet építeni
    std::cout<<(_game->getPlayer(0).towers().size() == 0)<<std::endl;
    std::cout<<(_game->getPlayer(1).towers().size() == 0)<<std::endl;
    std::cout<<(_game->getField(1, 5)->canBeEntered() == 1)<<std::endl;

    //Mindkét játékosnak megvan a pénze
    std::cout<<(_game->getPlayer(0).gold() == 1000)<<std::endl;
    std::cout<<(_game->getPlayer(1).gold() == 1000)<<std::endl;

    //Egy lerakásával a játékosa és a mezők is látják
    _game->getPlayer(0).placeTower(0, _game->getField(1, 5));
    std::cout<<(_game->getPlayer(0).towers().size() == 1)<<std::endl;
    std::cout<<(_game->getField(1, 5)->getTower()->owner() ==
                _game->players()[0])<<std::endl;
    std::cout<<(_game->getField(1, 5)->canBeEntered() == 0)<<std::endl;

    //Az ellenfél tornyainak száma változatlan, és nem tud ugyanoda helyezni
    std::cout<<(_game->getPlayer(1).towers().size() == 0)<<std::endl;
    _game->getPlayer(1).placeTower(0, _game->getField(1, 5));
    std::cout<<(_game->getField(1, 5)->getTower()->owner() ==
                _game->players()[0])<<std::endl;

    //A sikeres építés pénzbe kerül, a sikertelen nem
    std::cout<<(_game->getPlayer(0).gold() == 900)<<std::endl;
    std::cout<<(_game->getPlayer(1).gold() == 1000)<<std::endl;

    //Eredetileg nincsenek egységek
    //Egy indítása sikeres-e, a bázis mellől történik, és pénzbe kerül
    std::cout<<(_game->getPlayer(0).units().size() == 0)<<std::endl;
    _game->placeCreature(0);
    std::cout<<(_game->getPlayer(0).units().size() == 1)<<std::endl;
    std::cout<<(_game->getPlayer(0).units()[0]->location() == _game->getField(4, 0) ||
               _game->getPlayer(0).units()[0]->location() == _game->getField(6, 0) ||
               _game->getPlayer(0).units()[0]->location() == _game->getField(4, 1) ||
               _game->getPlayer(0).units()[0]->location() == _game->getField(5, 1) ||
               _game->getPlayer(0).units()[0]->location() == _game->getField(6, 1))<<std::endl;
    std::cout<<(_game->getPlayer(0).gold() == 800)<<std::endl;

    //Ha nincs elég pénz, se építeni, se egységet indítani nem lehet
    _game->getPlayer(0).spendGold(800);
    _game->placeCreature(0);
    std::cout<<(_game->getPlayer(0).units().size() == 1)<<std::endl;
    _game->getPlayer(0).placeTower(0, _game->getField(5, 8));
    std::cout<<(_game->getPlayer(0).towers().size() == 1)<<std::endl;

    _game->getPlayer(0).addGold(1000);
    _game->getPlayer(0).placeTower(0, _game->getField(5, 8)); //Az ellenfél egysége mellett
    _game->placeCreature(1);
    model::field *actual_loc = _game->getPlayer(0).units()[0]->location();

    //Update() hatására az egységek lépnek, a tornyok lőnek
    _game->update();
    std::cout<<(_game->getPlayer(0).units()[0]->location() != actual_loc)<<std::endl;
    std::cout<<(_game->getPlayer(1).units().size() == 0)<<std::endl;

    //Körönként, és egység elvesztéséért is megkapják az aranyat a játékosok
    std::cout<<(_game->getPlayer(0).gold() == 901)<<std::endl;
    std::cout<<(_game->getPlayer(1).gold() == 941)<<std::endl;

    //A tornyok csak 1 egységet tudnak lelőni 1 körben
    _game->placeCreature(1);
    _game->placeCreature(1);
    _game->update();
    std::cout<<(_game->getPlayer(1).units().size() == 1)<<std::endl;

    //Ha egy egység eléri az ellenfél bázisát, megsebzi azt, és meghal
    for(int i = 0; i < 11; i++){
        _game->update();
    }
    std::cout<<(_game->getPlayer(1).getBase()->HP() == 19)<<std::endl;
    std::cout<<(_game->getPlayer(0).units().size() == 0)<<std::endl;

    return 0;
}
