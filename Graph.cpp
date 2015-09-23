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

          size_t elem(degNeighborhood::const_iterator neighbor) {
              return neighbor->neighbor->elem;
          }

          int degree(const Neighborhood::const_iterator neighbors) {
              return degree(*neighbors);
          }


          degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, int elem){

              return find_if(neighborhood->begin(), neighborhood->end(), [elem](Neighbor& neighbor)->bool{
                  return neighbor.neighbor->elem == elem;
              });

               auto it = neighborhood->begin();
               while(it != neighborhood->end() && it->neighbor->elem != elem){
                   ++it;
               }
               return it;
          }

          degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, Vertices::iterator elem){
              return find_if(neighborhood->begin(), neighborhood->end(), [elem](Neighbor& neighbor)->bool{
                  return neighbor.neighbor == elem;
              });

          }

          Neighborhood::iterator Vertex::toNextList(Neighborhood::iterator list, degNeighborhood::iterator who)
           {    //DEBUG(this->elem, "-> Graph::toNextList(", *who, ")");
                assert(list != highNeighborhood());
                auto to_list = std::next(list);

                if(impl::degree(who)+1 < this->degree and
                (to_list == highNeighborhood() || impl::degree(to_list) != impl::degree(who)+1)) {
                    //MESSAGE("Creando la lista de grado", impl::degree(who), +"para albergar a", *who);
                    to_list = insertDegNeighborhood(to_list);
                }
                to_list->push_front(*who);
                MESSAGE("Borrando a", *who, "de su lista");
                erase(list, who);
                 DEBUG("END OF Graph::toNextList");
                 return to_list;
            }

                void Vertex::erase(Neighborhood::iterator list, degNeighborhood::iterator who) {
                    DEBUG(*this, "-> Graph::erase(", *who, ")");
                    list->erase(who);
                    if(list->empty() && list != highNeighborhood()) {
                        MESSAGE("Borrando la lista que quedo vacia");
                        neighborhood.erase(list);
                    }
                DEBUG("END OF Graph::erase");
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
            DUMP(*this);

            MESSAGE("grado de v: ", v->degree);
            MESSAGE("grado de w: ", w->degree);

            //PHASE 1: Algoritmo 1 del paper.
            DEBUG(std::string("Phase 1"));
            update_neighborhood(v);
            update_neighborhood(w);


            //PHASE 2:
            DEBUG(std::string("Phase 2"));
            Graph::Neighborhood::iterator v_list_in_w;
            Graph::Neighborhood::iterator w_list_in_v;
            // ya que se asegura que v tiene grado menor o igual a w se descarta contemplar el caso contrario
            if (w->degree > v->degree) {
                auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);

                //Si estoy en highN es porque no existe una lista de ese grado aún
                if (pos_v_in_low_w == w->highNeighborhood() || impl::degree(pos_v_in_low_w) > v->degree+1) {

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

            DUMP(*this);
            DEBUG("End of Graph::add_edge()");
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
         * APLICA LA FASE 1 AL VERTICE x.  Es decir.
         * Recorre cada w en el high neighborhood de v y actualiza la posicion de v dentro
         * del vecindario de w.  Para ello, debe pasar a v a la siguiente lista cuando
         * d(v) < d(w).  Cuando d(v) == d(w), debe bajar a N(v, d(v))
         */
        void Graph::update_neighborhood(Graph::Vertices::iterator x) {
            DEBUG(std::string("Graph::update_neighborhood(") + std::to_string(x->elem) + ")");
            DUMP(*this);

            for(auto it = x->highNeighborhood()->begin(); it != x->highNeighborhood()->end(); ++it){
                MESSAGE("Procesando high neighbor: ", it->neighbor->elem, "de grado", impl::degree(it));
                if(impl::degree(it) == x->degree) {
                    //Ya cree la lista que alberga los vertices de grado x?
                    Neighborhood::iterator to_list = std::prev(x->highNeighborhood());

                    if(x->highNeighborhood() == x->neighborhood.begin() || impl::degree(to_list) != x->degree) {
                        MESSAGE("Creando la lista de grado", x->degree);
                        to_list = x->insertDegNeighborhood(x->highNeighborhood());
                    } else {
                        to_list = std::prev(x->highNeighborhood());
                    }
                    to_list->push_front(*it);

                    //actualizo el list_pointer  y el self_pointer del neighbor de x en w
                    auto neighobor_x = it->self_pointer;// donde esta x en w

                    neighobor_x->list_pointer = to_list;
                    neighobor_x->self_pointer = to_list->begin();

                    //it = x->highNeighborhood()->erase(it);
                    x->highNeighborhood()->erase(it);
                    --it;
                } else {
                    it->list_pointer = it->neighbor->toNextList(it->list_pointer, it->self_pointer);
                    it->self_pointer = it->list_pointer->begin();
                }
                DUMP(*this);
            }

            DEBUG(std::string("END OF Graph::update_neighborhood(") + std::to_string(x->elem) + ")");
        }



    //     std::list<Graph::Neighbor>::iterator Graph::find_neighbor_in(Graph::Neighborhood::iterator neighborhood, int elem){
    //         auto it = neighborhood->begin();
    //         while(it != neighborhood->end() && it->neighbor->elem != elem){
    //             ++it;
    //         }
    //         return it;
    //     }

        /***
            hay que mover a v desde N(z, d(v)) hacia N(z,d(v)-1)

            remueve N(v,d(v)-1) para unirlo al highNeighborhood
        ***/

        void Graph::update_after_delete(Graph::Vertices::iterator x){
            DEBUG(std::string("BEGIN Graph::update_after_delete(") + std::to_string(x->elem) + ")");

            for(auto it = x->highNeighborhood()->begin(); it != x->highNeighborhood()->end(); ++it){
                DEBUG(std::string("Entrando al for Graph::update_after_delete(") + std::to_string(it->neighbor->elem) + ")");
                auto pos_x_in_w = it->neighbor->toPrevList(it->list_pointer, it->self_pointer);
                it->list_pointer = pos_x_in_w;
                it->self_pointer = pos_x_in_w->begin();
            }

            auto prev_high = prev(x->highNeighborhood());

            if(x->highNeighborhood() != x->neighborhood.begin() and impl::degree(prev_high) >= x->degree){

                for(auto n = prev_high->begin(); n != prev_high->end(); ++n){
                    x->highNeighborhood()->push_front(*n);
                    auto neighbor_x = n->self_pointer; // x en w
                    //actualizamos el neighbor_x que esta en w
                    neighbor_x->list_pointer = x->highNeighborhood();
                    neighbor_x->self_pointer = x->highNeighborhood()->begin();
                    n = prev_high->erase(n);
                    }

                x->neighborhood.erase(prev_high);
              }

            DEBUG(std::string("END Graph::update_after_delete(") + std::to_string(x->elem) + ")");
            DUMP(*this);
        }


        void Graph::removeEdge(Graph::const_vertex_iterator iter_v,Graph::const_vertex_iterator iter_w){
            auto v = to_iterator(iter_v);
            auto w = to_iterator(iter_w);

            /**** REVERTIR LA FASE 2 ****/
            if(v->degree > w->degree){
                std::swap(v,w);  // v es el de grado menor
            }
            // v seguro lo tiene a w en high, ya sea porque el es menor o porque tiene igual grado.
            auto neighbor = find_neighbor_in(v->highNeighborhood(), w);
            //borrar de low_w  a v
            w->erase(neighbor->list_pointer, neighbor->self_pointer);
            //borrar a w de la lista_v
            v->highNeighborhood()->erase(neighbor);

            /*** REVERTIR LA FASE 1 ***/
            v->degree -=1;
            w->degree -=1;

            //actualiza el vecindario
            update_after_delete(v);
            update_after_delete(w);
        }


    //    Graph::Neighborhood::iterator erase_neighborhood(Graph::Neighborhood::iterator){
    //
    //
    //    }

        /***
            Recorre todo el vecindario de v borrando únicamente a v del vecindario de sus vecinos
            y actualiza sólo el vecindario del otro vertice involucrado.
            Por ultimo se borra v de la lista de vertices.
        ***/

        void Graph::remove_vertex(Graph::const_vertex_iterator iter_v){
            DEBUG(std::string("BEGIN Graph::REMOVE_VERTEX (") + std::to_string(*iter_v) + ")");
            auto v = to_iterator(iter_v);
            for(auto list = v->neighborhood.begin(); list != v->neighborhood.end(); ++list) {
                for(auto it = list->begin(); it != list->end(); ++it) {
                    it->neighbor->erase(it->list_pointer,it->self_pointer);
                    it->neighbor->degree -=1;
                    update_after_delete(it->neighbor);
                }
            }

            vertices.erase(v);
            DEBUG(std::string("END Graph::REMOVE_VERTEX (") + std::to_string(*iter_v) + ")");
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

        Graph::deg_iterator Graph::H_begin(const_vertex_iterator v)const{
            return v.it->H_begin();
        }

        Graph::deg_iterator Graph::H_end(const_vertex_iterator v)const{
            return v.it->H_end();
        }

        Graph::const_neighbor_iterator Graph::N_begin(const_vertex_iterator v)const{
            return v.it->begin();
        }

        Graph::const_neighbor_iterator Graph::N_end(const_vertex_iterator v) const{
            return v.it->end();
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
                out << neighbor << ",";
            }
            out << "]  ";
        }
        return out << '>';
    }

    std::ostream& operator<<(std::ostream& out, const tip::impl::Neighbor& n) {
        return out << "(" << n.neighbor->elem << "," << n.self_pointer->neighbor->elem << ")";
    }

    std::ostream& operator<<(std::ostream& out, const tip::Graph& G) {
        return G.dump(out);
    }
