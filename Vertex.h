#include "VertexDescriptor.h"

#ifndef VERTEX_H
#define VERTEX_H
class Vertex{
public:
//constructor
Vertex(int id, VertexDescriptor *pointer);
const int id;
VertexDescriptor *vertexPointer;
};

#endif
