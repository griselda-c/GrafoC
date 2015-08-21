#include <iostream>
#include <list>
#include <algorithm>

#include "Graph.h"
#include "Log.h"

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
        DEBUG("Graph::add_edge(", v->elem, ",", w->elem, ")");
        DUMP("DUMP DEL GRAFO");
        DUMP(*this);

        MESSAGE("grado de v: ", v->degree);
        MESSAGE("grado de w: ", w->degree);

        //PHASE 1: Algoritmo 1 del paper.
        DEBUG(std::string("Phase 1"));
        update_neighborhood(v);
        update_neighborhood(w);

        DUMP("DUMP DEL GRAFO");
        DUMP(*this);


        //PHASE 2:
        DEBUG(std::string("Phase 2"));
        Graph::Neighborhood::iterator v_list_in_w;
        Graph::Neighborhood::iterator w_list_in_v;
        // ya que se asegura que v tiene grado menor o igual a w se descarta contemplar el caso contrario
        if (w->degree > v->degree) {
            auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);

            //Si estoy en highN es porque no existe una lista de ese grado aún
            if (pos_v_in_low_w == w->highNeighborhood()) {

                pos_v_in_low_w = w->insertDegNeighborhood(pos_v_in_low_w);
            }

            v_list_in_w = pos_v_in_low_w;
            w_list_in_v = v->highNeighborhood();
        }
        else {
             /** cuando se agrega el primer vertice,esto funciona **/
             v_list_in_w = w->highNeighborhood();
             w_list_in_v = v->highNeighborhood();
        }


        v_list_in_w->push_front(Neighbor(v));

        w_list_in_v->push_front(Neighbor(w));


        v_list_in_w->front().list_pointer = v->highNeighborhood();
        v_list_in_w->front().self_pointer = w_list_in_v->begin();

        w_list_in_v->front().self_pointer = v_list_in_w->begin();
        w_list_in_v->front().list_pointer = v_list_in_w;

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
        while(it != last && it->front().neighbor->degree < degree) {
            ++it;
            }

        return it;
    }

    /**
     * Recorre cada w en el high neighborhood de v y actualiza la posicion de v dentro
     * del vecindario de w.
     * FALTA ACTUALIZAR A W DENTRO DEL VECINDARIO DE V
     * si w esta en el high y tiene el mismo grado que v antes de agregar la arista
     * debe salir de High e ir a un N(v)
     */
    void Graph::update_neighborhood(Graph::Vertices::iterator x) {
        DEBUG(std::string("Graph::update_neighborhood(") + std::to_string(x->elem) + ")");

        //NOTA: NO PROCESAMOS EL HIGH NEIGHBORHOOD
        for(auto list = x->neighborhood.begin(); list != x->highNeighborhood(); ++list) {
            for(auto it = list->begin(); it != list->end(); ++it) {

                MESSAGE(std::string("Procesando vecino: ") + std::to_string(it->neighbor->elem));

                it->list_pointer = it->neighbor->toNextList(it->list_pointer, it->self_pointer);

                it->self_pointer = it->list_pointer->begin();
            }
        }

        updateHighNeighborhood(x);
        DEBUG(std::string("END OF Graph::update_neighborhood(") + std::to_string(x->elem) + ")");
    }



//     std::list<Graph::Neighbor>::iterator Graph::find_neighbor_in(Graph::Neighborhood::iterator neighborhood, int elem){
//         auto it = neighborhood->begin();
//         while(it != neighborhood->end() && it->neighbor->elem != elem){
//             ++it;
//         }
//         return it;
//     }

/**
        Busca en el higneighborhood de v aquellos que tienen el mismo grado de v,
        porque deben pasar a low, dado que v aumentará de grado.
        Busca el degneighborhood correcto, si no existe lo crea,
        agrega el Neighbor_w a la lista nueva, y lo borra del High,
        Por ultimo actualiza los punteros el Neighbor_x en w.

        Otro caso es aquellos que tienen un grado menos, eso quiere decir
        que w tiene a x en low, y ahora pasarian a tener igual grado por lo
        que debe mover a x a high_w

***/

    void Graph::updateHighNeighborhood(Graph::Vertices::iterator x){

        for(auto it = x->highNeighborhood()->begin(); it != x->highNeighborhood()->end(); ++it){

                if(impl::degree(it)== x->degree){

                    auto pos_new_it = find_neighborhood_with_degree(x,impl::degree(it));

                    if(impl::degree(pos_new_it) > impl::degree(it) || pos_new_it == x->highNeighborhood()){


                        pos_new_it = x->insertDegNeighborhood(pos_new_it);
                        pos_new_it->push_front(*it);

                        //actualizo el list_pointer  y el self_pointer del neighbor de x en w
                        auto neighobor_x = it->self_pointer;// donde esta x en w

                        neighobor_x->list_pointer = pos_new_it;
                        neighobor_x->self_pointer = pos_new_it->begin();

                        it =  x->highNeighborhood()->erase(it);
                    }
                }else if(impl::degree(it) == x->degree +1){
                    auto neighbor_x = it->self_pointer;
                    auto list_before = it->list_pointer;

                    it->list_pointer = it->neighbor->highNeighborhood();
                    it->list_pointer->push_front(*neighbor_x);
                    it->self_pointer = it->list_pointer->begin();

                    it->neighbor->erase(list_before, neighbor_x);
                }
        }
    }


    void Graph::update_after_delete(Graph::Vertices::iterator x){
        for(auto list_n = x->neighborhood.begin(); list_n != x->highNeighborhood(); ++list_n) {
            update_degNeighborhood(list_n,x);

            if(list_n->empty()){
                x->neighborhood.erase(list_n);
                --list_n;
            }

        }
    }

    std::list<Graph::degNeighborhood>::iterator Graph::update_degNeighborhood(std::list<Graph::degNeighborhood>::iterator list_n, Graph::Vertices::iterator x){
        for(auto it = list_n->begin(); it != list_n->end(); ++it) {
                it->list_pointer = it->neighbor->toPrevList(it->list_pointer, it->self_pointer);
                it->self_pointer = it->list_pointer->begin();

                if(impl::degree(it) == x->degree){

                auto neighbor_x = it->self_pointer; // x en w
                 x->highNeighborhood()->push_front(*it);
                //actualizamos el neighbor_x que esta en w
                neighbor_x->list_pointer = x->highNeighborhood();
                neighbor_x->self_pointer = x->highNeighborhood()->begin();

                it = list_n->erase(it);

                }
            }
        return list_n;

    }

    /***
        Funcion privada.
        Borra a v del vecindario de w y Borra a w del vecindario de v
        NO decrementa el grado NI actualiza el vecindario.
        Usado para remove_vertex y remove_edge
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
        //vertices.front().neighborhood.push_back(degNeighborhood());
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

    void Graph::print_vecinos(Graph::const_vertex_iterator x) const {
        auto v = x.it;

        for(auto neigh = v->neighborhood.begin(); neigh != v->neighborhood.end(); ++neigh){
            cout << "neighborhood size " +std::to_string (v->neighborhood.size())<< endl;
            cout << "degneighborhood size " +std::to_string (neigh->size())<< endl;
            for(auto deg_neig = neigh->begin(); deg_neig != neigh->end(); ++deg_neig){
                cout << "neighbor " +std::to_string (deg_neig->neighbor->elem)<< endl;
            }
        }
    }

    std::ostream& Graph::dump(std::ostream& out) const {
        for(auto& v : vertices) {
            out << v << std::endl;
        }
        return out;
    }


    namespace impl {
        std::string Vertex::dump() const {
            {
                std::string res;
                res += "elemento: " + std::to_string(elem) + '\n';
                res += "grado: " + std::to_string(degree) + '\n';
                for(auto it_degn = neighborhood.begin(); it_degn != neighborhood.end(); ++it_degn) {
                    if(it_degn != highNeighborhood()) {
                        res += "  Vecinos de grado: " + std::to_string(impl::degree(it_degn)) + '\n';
                    } else {
                        res += "  High neighborhood\n";
                    }
                    for(auto& neighbor : *it_degn) {
                        res += "    neighborhood: " + neighbor.dump();
                    }
                    res += '\n';
                }
                return res;
            }
        }
    }

};

std::ostream& operator<<(std::ostream& out, const tip::impl::Vertex& v) {
    out << '{' << v.elem << ", " << v.degree << '}';
    out << '<';
    for(auto it_degn = v.neighborhood.begin(); it_degn != v.neighborhood.end(); ++it_degn) {
        if(it_degn != v.highNeighborhood()) {
            out << "N(v," << tip::impl::degree(it_degn) << "): [";
        } else {
            out << "H(v): [";
        }
        for(auto& neighbor : *it_degn) {
            out << neighbor << ", ";
        }
        out << ']';
    }
    return out << '>';
}

std::ostream& operator<<(std::ostream& out, const tip::impl::Neighbor& n) {
    return out << "(" << n.neighbor->elem << "," << n.self_pointer->neighbor->elem << ")";
}

std::ostream& operator<<(std::ostream& out, const tip::Graph& G) {
    return G.dump(out);
}
