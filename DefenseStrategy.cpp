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

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){
   return Vector3((i * cellWidth) + cellWidth * 0.5f, (j * cellHeight) + cellHeight * 0.5f, 0); 
}

float distEuclidea(float x1, float y1, float x2, float y2){
  return sqrt(pow(x2-x1, 2)+pow(y2-y1, 2));
}

float cellValue(int i, int j, int iBase, int jBase, int nCellsWidth, int nCellsHeight, List<Object*> obstacles, List<Defense*> defenses) {

  float value = 0;

  int centerWidth = 0, centerHeight = 0;

  if(iBase == 0 && jBase == 0){
    centerWidth = nCellsWidth/2;
    centerHeight = nCellsHeight/2;
  }
  else{
    centerWidth = iBase;
    centerHeight = jBase;
  }

  if (i >= centerWidth)
    value += centerWidth - (i - centerWidth);
  else
    value += i+1;

  if (j >= centerHeight)
    value += centerHeight - (j - centerHeight);
  else
    value += j+1;
 /* Vector3 pos = cellCenterToPosition(i,j,cellWidth,cellHeight);  

  List<Object*>::iterator currentObstacle = obstacles.begin();
  int cont = 0;
  while(currentObstacle != obstacles.end()){
    float radio2 = (*currentObstacle)->radio;
    int x2 = (*currentObstacle)->position.x;
    int y2 = (*currentObstacle)->position.y;

    value -= distEuclidea(pos.x,pos.y,x2,y2);
     
      return false;
    }

    currentObstacle++;
    cont++;
  }*/

	return value; // implemente aqui la función que asigna valores a las celdas
}

void allCellValues(multimap<float,vector<int>,greater<int> >& values, int iBase, int jBase, int nCellsWidth, int nCellsHeight
  , List<Object*> obstacles, List<Defense*> defenses) {
    int valoracion = 0;
    vector<int> celda(2);
    for (int i = 0; i < nCellsWidth; ++i){
      for (int j = 0; j < nCellsHeight; ++j){
        valoracion = cellValue(i, j, iBase, jBase, nCellsWidth, nCellsHeight, obstacles, defenses);
        celda[0] = i;
        celda[1] = j;
        values.insert(make_pair(valoracion, celda));
        cout<<valoracion<<" ";
      }
      cout<<endl<<endl;
    }
}

vector<int> extraeCandidato(multimap<float,vector<int>,greater<int> >& values){

  multimap<float,vector<int>,greater<int> >::iterator p  = values.begin();
  vector<int> bestCell = p->second;
  values.erase(p);

  return bestCell;
}

bool esFactible(vector<int> cell,bool** freeCells,Defense* defense, std::list<Defense*> defenses, List<Object*> obstacles, float cellWidth, float cellHeight, int nCellsWidth, int nCellsHeight){
  float radio = defense->radio;
  int i = cell[0], j = cell[1];
  Vector3 pos = cellCenterToPosition(i,j,cellWidth,cellHeight);

  // Comprobar que está dentro de los límites
  if(pos.x-radio < 0 || pos.x+radio > ((nCellsWidth*cellWidth)-50) || pos.y-radio < 0 || pos.y+radio > ((nCellsHeight*nCellsHeight)-50))
    return false;

  // Comprobar si toca con obstaculo o defensa

  List<Object*>::iterator currentObstacle = obstacles.begin();
  int cont = 0;
  while(currentObstacle != obstacles.end()){
    float radio2 = (*currentObstacle)->radio;
    int x2 = (*currentObstacle)->position.x;
    int y2 = (*currentObstacle)->position.y;

    if (distEuclidea(pos.x,pos.y,x2,y2) < (radio + radio2 + 1)){
     /* cout<<"Obstacle "<<cont<<": "<<x2<<", "<<y2<<"     ";
      cout<<"Distancia:  "<<distEuclidea(pos.x,pos.y,x2,y2)<<" >"<<(radio+radio2)<<endl;*/
      return false;
    }

    currentObstacle++;
    cont++;
  }

  List<Defense*>::iterator currentDefense = defenses.begin();
  cont = 0;

  while(*currentDefense != defense){
    float radio2 = (*currentDefense)->radio;
    int x2 = (*currentDefense)->position.x;
    int y2 = (*currentDefense)->position.y;

    //cout<<"Torre "<<cont<<": "<<x2<<", "<<y2<<"     ";
    

    if (distEuclidea(pos.x,pos.y,x2,y2) < (radio + radio2)){
      //cout<<"Distancia:  "<<distEuclidea(pos.x,pos.y,x2,y2)<<" >"<<(radio+radio2)<<" XXX"<<endl<<endl;
      return false;
    }

     // cout<<"Distancia:  "<<distEuclidea(pos.x,pos.y,x2,y2)<<" >"<<(radio+radio2)<<endl;


    currentDefense++;
    cont++;
  }

 //freeCells[i][j] = false;

  return true;
}

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

    // Define la matriz de valores
    multimap<float,vector<int>,greater<int> > values;

    // Valora cada celda del mapa
    allCellValues(values, 0, 0, nCellsWidth, nCellsHeight, obstacles, defenses);

    /*for (int i = 0; i < nCellsWidth; ++i)
    {
      for (int j = 0; j < nCellsHeight; ++j)
      {
        cout<<freeCells[i][j]<<"  ";
      }
      cout<<"\n\n";
    }*/




    /*for(multimap<float,vector<int>,greater<int> >::iterator p  = values.begin(); p != values.end(); p++){
      cout<<p->first<<endl;
    }*/
    int iBase = 0, jBase = 0;
    int cont = 0;
    cout<<"DEFENSE "<<cont<<"\n\n";
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end()) {

        // Extrae la celda mas prometedora
        vector<int> bestCell = extraeCandidato(values);
        Vector3 pos = cellCenterToPosition(bestCell[0],bestCell[1],cellWidth,cellHeight);
        if(esFactible(bestCell, freeCells, *currentDefense, defenses, obstacles, cellWidth, cellHeight, nCellsWidth, nCellsHeight)){
          if(cont == 0){
            iBase = bestCell[0];
            jBase = bestCell[1];
            allCellValues(values, iBase, jBase, nCellsWidth, nCellsHeight, obstacles, defenses);
          }

          cout<<"Posicion: "<<pos.x<<" "<<pos.y<<endl<<endl;
          (*currentDefense)->position.x = pos.x;
          (*currentDefense)->position.y = pos.y;
          (*currentDefense)->position.z = 0; 
          ++currentDefense;
          cont++;
          cout<<"DEFENSE "<<cont<<"\n\n";
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
