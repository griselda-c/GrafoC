#include <iostream>
#include <list>

#include "Graph.h"

using namespace std;

namespace tip {

    void Graph::add_edge(Graph::const_vertex_iterator v, Graph::const_vertex_iterator w){

        if(is_greater_degree(v,w)){
            create_relationship(v,w, false);
        }
        else if(is_equal_degree(v,w)) {
                create_relationship(v,w, true);
            }
            else {
                create_relationship(w,v, false);
            }

        //cambiarGrado

        //actualizar punteros

    };

    void Graph::create_relationship(Graph::const_vertex_iterator greater_v, Graph::const_vertex_iterator lower_w, bool equally) {
        // El nuevo Neighbor de v
        Graph::Neighbor v_neighbor = Neighbor();
        v_neighbor.neighbor = lower_w.it->elem;

        // El nuevo Neighbor de w
        Graph::Neighbor w_neighbor = Neighbor();
        w_neighbor.neighbor = greater_v.it->elem;

        insert_in_highn(lower_w, greater_v, v_neighbor);

        // Si tienen el mismo grado los dos van al H(N) de cada uno
        // por lo tanto se actualiza el H(N) de v agregando el Neighbor de w
        if (equally) {
            insert_in_highn(greater_v, lower_w, w_neighbor);
        }
        // En caso contrario el Neighbor de w se agrega al L(N) de v
        else {
            int w_new_degree = lower_w.it->degree + 1;
            std::list<Graph::Neighborhood> low_n = greater_v.it->lowNeighborhood;
            for (auto it = low_n.begin(); it != low_n.end(); ++it) {
                if (it->front().degree == w_new_degree) {
                    it->push_back(w_neighbor);
                    w_neighbor.self_pointer = it;
                    //w_neighbor.list_pointer = low_n.begin();
                }
            }
        }

    }

    void Graph::insert_in_highn(Graph::const_vertex_iterator v, Graph::const_vertex_iterator w, Graph::Neighbor nv) {
        // Se actualiza el H(N) de v agregando el Neighbor de w
        Graph::Neighborhood high_v = v.it->highNeighborhood;
        high_v.push_back(nv);

        // Se actualiza el Neighbor de w indicandole donde se encuentra dentro de v
        for (auto it = high_v.begin(); it != high_v.end(); ++it) {
            if (it->elem == nv) {
                nv.self_pointer = it;
                nv.list_pointer = high_v.begin();
            }
        }
    }
    
     void Graph::removeEdge(Graph::const_vertex_iterator  v, Graph::const_vertex_iterator  w){
        decreaseDegree(v);
        decreaseDegree(w);

        //if existe edge(v,w)
         deleteNeighbor(v,w); //w es vecino de v
         //update_after_delete();

        }
        
        void Graph::deleteNeighbor(Graph::const_vertex_iterator v,Graph::const_vertex_iterator w){
        auto v1 = vertices.erase(v.it,v.it);
        auto w1= vertices.erase(w.it,w.it);

    if(is_greater_degree(v,w)){ // w esta en low
           auto list_low= v.it->lowNeighborhood;
        //borrar w de low
         for(auto it = list_low.begin(); it != list_low.end(); ++it) {//lista de listas

             for(auto it_neighbor = it->begin(); it_neighbor != it->end(); ++it_neighbor ){//lista
                if(it_neighbor->neighbor == w.it->elem){
                    //borra a v en w
                    w1->highNeighborhood.erase(it_neighbor->self_pointer);
                    v1->lowNeighborhood;
                 }
             }//for de lista
         } // for de listas de listas
     }else if(is_equal_degree(v,w)){ // borrar w de high
         auto list_high = v.it->highNeighborhood;
          for(auto it2 = list_high.begin(); it2 != list_high.end(); it2++){
                if(it2->neighbor == w.it->elem){
                  w1->highNeighborhood.erase(it2->self_pointer);
                  v1->highNeighborhood;
               }
        }

     }else{
        auto list_high = v.it->highNeighborhood;
          for(auto it2 = list_high.begin(); it2 != list_high.end(); it2++){
                if(it2->neighbor == w.it->elem){

                  it2->list_pointer->erase(it2->self_pointer);
                  //w1->lowNeighborhood.erase(it2->self_pointer);
                  v1->highNeighborhood;
               }
        }
     }//else

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
    
    Graph::const_vertex_iterator Graph::increaseDegree(const_vertex_iterator v1){
        auto ptr2 = vertices.erase(v1.it,v1.it);
        ptr2->degree+=1;
        return v1;
     };

     Graph::const_vertex_iterator Graph::decreaseDegree(const_vertex_iterator v1){
        auto ptr2 = vertices.erase(v1.it,v1.it);
        ptr2->degree-=1;
        return v1;
     };

    
    Graph::const_vertex_iterator Graph::insertVertex(unsigned int elem) {
        vertices.push_front(Vertex(elem));
        return begin();
    }

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

};

