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

float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	return col; // implemente aqui la función que asigna valores a las celdas
}

void allCellValues(map<int,float>& values, bool** freeCells, int nCellsWidth, int nCellsHeight
  , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    int valoracion = 0;
    //for (int i = 0; i < nCellsHeight; ++i){
      for (int j = 0; j < nCellsWidth; ++j){
        valoracion = cellValue(0, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
        values.insert(make_pair(j, valoracion));
      }
   // }
}


/*void ordenaCandidatos(float** values){

}

int* seleccionaCandidato(const float** values){

}*/

/*bool isValid(int row, int col, ){

}*/

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

    // Define la matriz de valores
    map<int, float> values;

    // Valora cada celda del mapa
    allCellValues(values, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
    
    map<int, float>::iterator p  = values.begin();
    p++;
    cout<<"\n\n\n\n\n\n";
    cout<<p->second<<endl;
    
/*
    int maxAttemps = 1000;
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end() && maxAttemps > 0) {

        // Selecciona la celda mas prometedora


        (*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
        (*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
        (*currentDefense)->position.z = 0; 
        ++currentDefense;
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

#endif*/
}
