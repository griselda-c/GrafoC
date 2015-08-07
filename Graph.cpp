#include <iostream>
#include <list>
#include <algorithm>

#include "Graph.h"

using namespace std;

namespace tip {

  namespace impl {
      int degree(degNeighborhood::const_iterator neighbor) {
          return neighbor->neighbor->degree;
      }

      int degree(const degNeighborhood& neighbors) {
          return degree(neighbors.begin());
      }

      int degree(const Neighborhood::const_iterator neighbors) {
          return degree(*neighbors);
      }


      degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, int elem){

          return find_if(neighborhood->begin(), neighborhood->end(), [elem](auto& neighbor)->bool{
              return neighbor.neighbor->elem == elem;
          });

           auto it = neighborhood->begin();
           while(it != neighborhood->end() && it->neighbor->elem != elem){
               ++it;
           }
           return it;
      }

      degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, Vertices::iterator elem){
          return find_if(neighborhood->begin(), neighborhood->end(), [elem](auto& neighbor)->bool{
              return neighbor.neighbor == elem;
          });

      }

  }


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
        //auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);

        Graph::degNeighborhood v_list_in_w;
        Graph::degNeighborhood w_list_in_v;

        // ya que estar aseguro que v tiene grado menor o igual a w se descarta contemplar el caso contrario
        if (w->degree > v->degree) {
            //pos_v_in_low_w = w->neighborhood.insert(pos_v_in_low_w, Neighborhood());
            auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);

            //if pos_v_in_low_w.begin().degree > v->degree + 1
            pos_v_in_low_w = w->insertDegNeighborhood(pos_v_in_low_w);

            auto& v_list_in_w = *pos_v_in_low_w;
            auto& w_list_in_v = *v->highNeighborhood();

            w_list_in_v.back().list_pointer = pos_v_in_low_w;
        }
        else {
            auto& v_list_in_w = *w->highNeighborhood();
            auto& w_list_in_v = *v->highNeighborhood();

            w_list_in_v.back().list_pointer = w->highNeighborhood();
        }


        v_list_in_w.push_front(Neighbor(v));
        w_list_in_v.push_front(Neighbor(w));

        v_list_in_w.back().list_pointer = v->highNeighborhood();
        v_list_in_w.back().self_pointer = w_list_in_v.begin();

        w_list_in_v.back().self_pointer = v_list_in_w.begin();

        v->degree +=1;
        w->degree +=1;

    };

    /**
     * Retorna un puntero a la primer lista del low_neighbhood de w que tiene grado al menos degree.
     */
    std::list<Graph::degNeighborhood>::iterator Graph::find_neighborhood_with_degree(Graph::Vertices::iterator w, int degree)
    {
        //TODO: reemplazar por find_if
        std::list<Graph::degNeighborhood>::iterator first = w->neighborhood.begin();
        return find_neighborhood_with_degree(first, std::prev(w->neighborhood.end()), degree);
    }

    /**
     * TODO: REEMPLAZAR POR find_if DEL STD
     */
    std::list<Graph::degNeighborhood>::iterator Graph::find_neighborhood_with_degree(
        std::list<Graph::degNeighborhood>::iterator first,
        std::list<Graph::degNeighborhood>::iterator last,
        int degree)
    {
//        return find_if(first, last, [degree](auto& list_n)->bool{
//              return list_n.front().neighbor->degree == degree;
//          });
        auto it = first;
        while(it != last ) {
            if(!it->empty() and it->front().neighbor->degree < degree){
                return it;
            }
            ++it;
        }
        return it;
    }

    /**
     * Recorre cada w en el high neighborhood de v y actualiza la posicion de v dentro
     * del vecindario de w.
     */
    void Graph::update_neighborhood(Graph::Vertices::iterator x) {
        for(auto list = x->neighborhood.begin(); list != x->neighborhood.end(); ++list) {
            for(auto it = list->begin(); it != list->end(); ++it) {
                it->list_pointer = it->neighbor->toNextList(it->list_pointer, it->self_pointer);
                it->self_pointer = it->list_pointer->begin();
            }
        }
    }


//     std::list<Graph::Neighbor>::iterator Graph::find_neighbor_in(Graph::Neighborhood::iterator neighborhood, int elem){
//         auto it = neighborhood->begin();
//         while(it != neighborhood->end() && it->neighbor->elem != elem){
//             ++it;
//         }
//         return it;
//     }


    void Graph::update_after_delete(Graph::Vertices::iterator x){
        for(auto list = x->neighborhood.begin(); list != x->neighborhood.end(); ++list) {
            for(auto it = list->begin(); it != list->end(); ++it) {
                it->list_pointer = it->neighbor->toPrevList(it->list_pointer, it->self_pointer);
                it->self_pointer = it->list_pointer->begin();
            }
        }
    }

    /***
        Funcion privada.
        Borra a v del vecindario de w y Borra a w del vecindario de v
        NO decrementa el grado NI actualiza el vecindario.
    ***/

    void Graph::remove_edge(Graph::Vertices::iterator v, Graph::Vertices::iterator w){
         if(v->degree > w->degree){
            std::swap(v,w);  // v es el de grado menor
        }
        // v seguro lo tiene a w en high, ya sea porque el es menor o porque tiene igual grado.
        auto neighbor = find_neighbor_in(v->highNeighborhood(), w);
        //borrar de low_w  a v
        w->erase(neighbor->list_pointer, neighbor->self_pointer);
        //borrar a w de la lista_v
        v->highNeighborhood()->erase(neighbor);

    }

    void Graph::removeEdge(Graph::const_vertex_iterator iter_v,Graph::const_vertex_iterator iter_w){
        auto v = to_iterator(iter_v);
        auto w = to_iterator(iter_w);

        /**** REVERTIR LA FASE 2 ****/
        remove_edge(v,w);

        /*** REVERTIR LA FASE 1 ***/
        v->degree -=1;
        w->degree -=1;

        //actualiza el vecindario
        update_after_delete(v);
        update_after_delete(w);
    }

    /***
        Recorre todo el vecindario de v borrando las aristas existentes
        y actualiza sólo el vecindario del otro vertice involucrado.
        Por ultimo se borra v de la lista de vertices.
    ***/

    void Graph::remove_vertex(Graph::const_vertex_iterator iter_v){
        auto v = to_iterator(iter_v);
        for(auto list = v->neighborhood.begin(); list != v->neighborhood.end(); ++list) {
            for(auto it = list->begin(); it != list->end(); ++it) {
                remove_edge(v,it->neighbor);
                it->neighbor->degree -= 1;
                update_after_delete(it->neighbor);
            }
        }

        vertices.erase(v);
    }


    int Graph::vertexCount() const {
        return  this->vertices.size();
    };

    int Graph::degree(Graph::const_vertex_iterator v) const{
        return v.it->degree;
    };


    Graph::const_vertex_iterator Graph::insertVertex(unsigned int elem) {
        vertices.push_front(Vertex(elem));
        vertices.front().neighborhood.push_back(degNeighborhood());
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

