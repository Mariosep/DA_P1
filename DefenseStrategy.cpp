// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################
#include <map> 
#include <iostream>

#define BUILDING_DEF_STRATEGY_LIB 1
#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace std;
using namespace Asedio;

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); }

float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {

	return 1000-row-col; // implemente aqui la función que asigna valores a las celdas
}

void allCellValues(multimap<float,vector<int>,greater<int> >& values, bool** freeCells, int nCellsWidth, int nCellsHeight
  , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    int valoracion = 0;
    vector<int> celda(2);
    for (int i = 0; i < nCellsHeight; ++i){
      for (int j = 0; j < nCellsWidth; ++j){
        valoracion = cellValue(0, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
        celda[0] = i;
        celda[1] = j;
        values.insert(make_pair(valoracion, celda));
      }
    }
}

vector<int> extraeCandidato(multimap<float,vector<int>,greater<int> >& values){

  multimap<float,vector<int>,greater<int> >::iterator p  = values.begin();
  vector<int> bestCell = p->second;
  values.erase(p);

  return bestCell;
}

bool esFactible(vector<int> cell,bool** freeCells,Defense* defense, int nCellsWidth, int nCellsHeight){
  int radio = defense->radio;
  int i = cell[0], j = cell[1];
  // Comprobar que está dentro de los límites
  if(i-radio < 0 || i+radio > nCellsWidth || j-radio < 0 || j+radio > nCellsHeight)
    return false;

  // Comprobar si toca con obstaculo o defensa
  if(freeCells[i,j] == false)
    return false;

  for(int despl=1; despl<radio; despl++){
    if((freeCells[i-despl,j] && freeCells[i,j-despl] && freeCells[i-despl,j-despl] && freeCells[i+despl,j] && freeCells[i,j+despl]
        && freeCells[i+despl,j+despl] && freeCells[i-despl,j+despl] && freeCells[i+despl,j-despl]) == false)
      return false;
  }

  // Actualizar freeCells con la nueva defensa
  freeCells[i-despl,j] = false;
  freeCells[i,j-despl] = false;
  freeCells[i-despl,j-despl] = false;
  freeCells[i+despl,j] = false;
  freeCells[i,j+despl] = false;
  freeCells[i+despl,j+despl] = false;
  freeCells[i-despl,j+despl] = false;
  freeCells[i+despl,j-despl] = false;

  return true;
}

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

    // Define la matriz de valores
    multimap<float,vector<int>,greater<int> > values;

    // Valora cada celda del mapa
    allCellValues(values, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
    
    for (int i = 0; i < nCellsHeight; ++i)
    {
      for (int j = 0; j < nCellsWidth; ++j)
      {
        cout<<freeCells[i][j]<<"  ";
      }
      cout<<"\n\n";
    }


    /*for(multimap<float,vector<int>,greater<int> >::iterator p  = values.begin(); p != values.end(); p++){
      cout<<p->first<<endl;
    }*/
    
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end()) {

        // Extrae la celda mas prometedora
        vector<int> bestCell = extraeCandidato(values);
        Vector3 pos = cellCenterToPosition(bestCell[0],bestCell[1],cellWidth,cellHeight);
        if(esFactible(bestCell, freeCells, *currentDefense, nCellsWidth, nCellsHeight)){
          cout<<pos.x<<" "<<pos.y<<endl;
          (*currentDefense)->position.x = ((int)(bestCell[0])) * cellWidth + cellWidth * 0.5f;
          (*currentDefense)->position.y = ((int)(bestCell[1])) * cellHeight + cellHeight * 0.5f;
          (*currentDefense)->position.z = 0; 
          ++currentDefense;
        }
    }

#ifdef PRINT_DEFENSE_STRATEGY

    float** cellValues = new float* [nCellsHeight]; 
    for(int i = 0; i < nCellsHeight; ++i) {
       cellValues[i] = new float[nCellsWidth];
       for(int j = 0; j < nCellsWidth; ++j) {
           cellValues[i][j] = ((int)(cellValue(i, j))) % 256;
       }
    }
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
}
