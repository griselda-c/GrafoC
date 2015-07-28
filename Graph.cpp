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
        if(pos_v_in_low_w == w->lowNeighborhood.end()) {
            if (pos_v_in_low_w->neighbor->degree > v->degree + 1) {
                pos_v_in_low_w = w->lowNeighborhood.insert(pos_v_in_low_w, Neighborhood());

                auto& v_list_in_w = *pos_v_in_low_w;
                auto& w_list_in_v = v->highNeighborhood;

                w_list_in_v.back().list_pointer = pos_v_in_low_w;
            }
            else if (w->degree + 1 == v->degree + 1) {
                auto& v_list_in_w = w->highNeighborhood;
                auto& w_list_in_v = v->highNeighborhood;

                w_list_in_v.back().list_pointer = w->lowNeighborhood.end();
            }
        }

        v_list_in_w.push_front(Neighbor(v));
        w_list_in_v.push_front(Neighbor(w));

        v_list_in_w.back().list_pointer = v->lowNeighborhood.end();
        v_list_in_w.back().self_pointer = w_list_in_v.begin();

        w_list_in_v.back().self_pointer = v_list_in_w.begin();

        //cambiarGrado

    };

    /**
     * Retorna un puntero a la primer lista del low_neighbhood de w que tiene grado al menos degree.
     */
    std::list<Graph::Neighborhood>::iterator Graph::find_neighborhood_with_degree(Graph::Vertices::iterator w, int degree)
    {
        //TODO: reemplazar por find_if
        std::list<Graph::Neighborhood>::iterator first = w->lowNeighborhood.begin();
        return find_neighborhood_with_degree(first, w->lowNeighborhood.end(), degree);
    }

    /**
     * TODO: REEMPLAZAR POR find_if DEL STD
     */
    std::list<Graph::Neighborhood>::iterator Graph::find_neighborhood_with_degree(
    std::list<Graph::Neighborhood>::iterator first,
    std::list<Graph::Neighborhood>::iterator last,
    int degree)
    {
        auto it = first;
        while(it != last && it->front().neighbor->degree < degree) {
            ++it;
        }
        return it;
    }

    
    void update_neighborhood(Graph::Vertices::iterator v) {
        auto it = v->highNeighborhood.begin();
        while(it != v->highNeighborhood.end()){
            if (it->neighbor->degree != v->degree) {
                v_list_in_x = it->list_pointer;
                v_list_in_x->remove(*(it->self_pointer));
                ++v_list_in_x;
                it->list_pointer = v_list_in_x;
                v_list_in_x->push_front(*(it->self_pointer));
            }
            else {
                // Nota: falta contemplar el caso en que tenían mismo grado

            }
        }
    }


    void Graph::removeEdge(Graph::const_vertex_iterator  v, Graph::const_vertex_iterator  w){


        //if existe edge(v,w)
         deleteNeighbor(v,w); //w es vecino de v
        //update_after_delete();

    }

    std::list<Graph::Neighbor>::iterator Graph::find_neighbor_in_high(Graph::Vertices::iterator v,int elem){
        auto it = v->highNeighborhood.begin();
        while(it != v->highNeighborhood.end() && it->neighbor->elem == elem){
            ++it;
        }
            return it;
    }

    void Graph::deleteNeighbor(Graph::const_vertex_iterator iter_v,Graph::const_vertex_iterator iter_w){
        auto v = to_iterator(iter_v);
        auto w = to_iterator(iter_w);
        if(v->degree > w->degree){
            std::swap(v,w);  // v es el de grado menor
        }

        if(v->degree == w->degree){
        // w esta en high_v
        auto neighbor = find_neighbor_in_high(v,w->elem);
        //borrar a v de w
        w->highNeighborhood.erase(neighbor->self_pointer);
        //borrar a w de v
        v->highNeighborhood.erase(neighbor);

        }else{

        auto neighbor = find_neighbor_in_high(v,w->elem);
        //buscar a v en low_w
        *neighbor->list_pointer->erase(neighbor->self_pointer);
        //borrar a w en v
        v->highNeighborhood.erase(neighbor);

        }
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

