#include "VertexDescriptor.h"

#ifndef VERTEX_H
#define VERTEX_H
class Vertex{

//constructor
Vertex(int id, VertexDescriptor *pointer);



private:
  const int id;
  VertexDescriptor *vertexPointer;
};

#endif
