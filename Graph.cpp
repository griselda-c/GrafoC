#include <iostream>
#include <list>

#include "Graph.h"

using namespace std;

namespace tip {

    void Graph::addEdge(Graph::const_vertex_iterator v, Graph::const_vertex_iterator w){
       //cambiarGrado
        if(is_greater_degree(v,w)){
            //Construir Neighbord
            createRelationship(v,w, false);
            //v guardar el neighbor w en Low
            //w guardar el neigbor v en High
        } else if(is_equal_degree(v,w)) {
            createRelationship(v,w, true);
            } else {
                //construir otro Neigbord
                createRelationship(w,v, false);
                //v guarda a neighbor de w en High
                //w guardar el neigbor v en Low
            }

    };

    void Graph::createRelationship(Graph::const_vertex_iterator greater_v, Graph::const_vertex_iterator lower_w, bool equally) const {
        Graph::Neighbor v_neighbor = Neighbor();
        v_neighbor.neighbor = lower_w.it->elem;

        Graph::Neighbor w_neighbor = Neighbor();
        w_neighbor.neighbor = lower_v.it->elem;

        lower_w.it->highNeighborhood.push_back(v_neighbor);
        v_neighbor.self_pointer =

        if (equally) {
            greater_v.it->highNeighborhood.push_back(w_neighbor);
            w_neighbor.self_pointer =
        } else {
            int w_degree = lower_w.it->degree;
            lower_v.it->lowNeighborhood.
        }

    }

    int Graph::vertexCount() const {
        return  this->vertices.size();
    };

   int Graph::degree(Graph::const_vertex_iterator v) const{
         return v.it->degree;
     };

    bool Graph::is_greater_degree(Graph::const_vertex_iterator v1,Graph::const_vertex_iterator v2){
           return v1.it->degree > v2.it->degree;
//             return v1.it.is_degree_greater(v2);
     }

    bool Graph::is_equal_degree(Graph::const_vertex_iterator v1,Graph::const_vertex_iterator v2){
           return v1.it->degree == v2.it->degree;
//             return v1.it.is_degree_greater(v2);
     }

    Graph::const_vertex_iterator Graph::cambiarDegree(const_vertex_iterator v1){
        auto ptr2 = vertices.erase(v1.it,v1.it);
        ptr2->degree+=1;
        return v1;
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


    ///////////////////////////*******************************//////////////////////
//    Graph::vertex_iterator Graph::begin()  {
//        return vertex_iterator(vertices.begin());
//    }
//
//    Graph::vertex_iterator Graph::cbegin()  {
//        return begin();
//    }
//
//    Graph::vertex_iterator Graph::end()  {
//        return vertex_iterator(vertices.end());
//    }
//
//    Graph::vertex_iterator Graph::cend()  {
//        return end();
//    }
//

}

