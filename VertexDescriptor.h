#include <list>
#include "NeighborDescriptor.h"

#ifndef VERTEXDESCRIPTOR_H
#define VERTEXDESCRIPTOR_H

class VertexDescriptor{
    public:
        int degree;
        list <NeighborDescriptor> highNeighborhood;
        list<list<NeighborDescriptor>> lowNeighborhood;
};
#endif // VERTEXDESCRIPTOR_H
