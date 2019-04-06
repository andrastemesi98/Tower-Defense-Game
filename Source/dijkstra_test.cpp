#include "../Include/game.h"
#include "../Include/field.h"
#include "../Include/path.h"
#include <iostream>

void print(model::game& g, std::vector<model::field*> *way = nullptr)
{
  if(way != nullptr)
    std::cout << "Dijkstra from (" << (*way)[0]->x << ","<< (*way)[0]->y << ") to ("
	      << (*way).back()->x << "," << (*way).back()->y << ")\n";
  for(uint i = 0; i < g.sizey(); ++i)
    {
      for(uint j = 0; j < g.sizex(); ++j)
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
	  else std::cout << "1";
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

int main()
{
  model::game g(5, 7);
  print(g);
  
  model::field* start = g.getField(2,1);
  model::field* goal = g.getField(3,6);
  std::vector<model::field*> fs;
  
  bool b = model::dijkstra(fs, start, goal);
  if(!b)
    std::cerr << "Hibás dijkstra: nem talál utat\n";
  printpath(fs);
  print(g, &fs);
  
}
