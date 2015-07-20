#include <iostream>
#include <list>

#include "Graph.h"

using namespace std;

namespace tip {

    int Graph::vertexCount() const {
        return  this->vertices.size();
    };

     int Graph::degree(Graph::const_vertex_iterator v) const{
         return v.degree();

     };

    Graph::const_vertex_iterator Graph::insertVertex(unsigned int elem) {
        vertices.push_front(Vertex(elem));
        return begin();
    };

    Graph::const_vertex_iterator Graph::begin() const {
        return const_vertex_iterator(vertices.begin());
    }

    Graph::const_vertex_iterator Graph::cbegin() const {
        return begin();
    }

    Graph::const_vertex_iterator Graph::end() const {
        return const_vertex_iterator(vertices.end());
    }

    Graph::const_vertex_iterator Graph::cend() const {
        return end();
    }


}

