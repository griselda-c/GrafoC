#include <list>
using namespace std;
#include "NeighborDescriptor.h"

#ifndef VERTEXDESCRIPTOR_H
#define VERTEXDESCRIPTOR_H
class VertexDescriptor{

int degree;
list <NeighborDescriptor> highNeighborhood;
list<list<NeighborDescriptor>> lowNeighborhood;
};
#endif // VERTEXDESCRIPTOR_H
