#include "Graph.h"
#include <iostream>
#include <list>
using namespace std;

 int Graph::vertexCount(){
   return  this->vertexs.size();
};

int* Graph::degree(Vertex *v){
  return &v->vertexPointer->degree;

};




