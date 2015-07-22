#include <iostream>
#include <list>

#include "Graph.h"

using namespace std;

namespace tip {

    void Graph::add_edge(Graph::const_vertex_iterator iter_v, Graph::const_vertex_iterator iter_w){

        auto v = to_iterator(iter_v);
        auto w = to_iterator(iter_w);

        //queremos que v tenga menor o igual grado a w
        if(v->degree > w->degree){
            std::swap(v,w);
        }

        //PHASE 1: Algoritmo 1 del paper.
        update_neighborhood(v);
        update_neighborhood(w);

        //PHASE 2:
        //Tener cuidado con que v y w tengan el mismo grado.
        auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);
        if(pos_v_in_low_w == w->low_neghborhood.end() || pos_v_in_low_w->degree > v->degree + 1) {
            pos_v_in_low_w  = low_neighbhood.insert(pos_v_in_low_w, Neighborhood());
        }
        
        auto& v_list_in_w = *pos_v_in_low_w;
        auto& w_list_in_v = v->high_neighborhood;
        
        v_list_in_w.push_front(Neighbhor(v));
        w_list_in_v.push_front(Neighbhor(w));
        
        v_list_in_w.back().self_pointer = w_list_in_v.begin();
        v_list_in_w.back().list_pointer = v->low_neighborhood.end();
        
        w_list_in_v.back().self_pointer = v_list_in_w.begin();
        w_list_in_v.back().list_pointer = pos_v_in_low_n;

        //cambiarGrado

    };

    /**
     * Retorna un puntero a la primer lista del low_neighbhood de w que tiene grado al menos degree.
     */
    std::list<Graph::Neighborhood>::iterator find_neighborhood_with_degree(Vertices::iterator w, int degree)
    {
        return find_neighborhood_with_degree(w->low_neighbhood.begin(), w->low_neighbhood.end(), degree);
    }
    
    /**
     *
     */
    std::list<Graph::Neighborhood>::iterator find_neighborhood_with_degree(
       std::list<Graph::Neighborhood>::iterator first,
       std::list<Graph::Neighborhood>::iterator last,
       int degree)
    {
        auto it = first
        while(it != last && it->front().degree < degree) {
            ++it;
        }
        return it;
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

