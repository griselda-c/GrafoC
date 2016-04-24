
#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include <cassert>

#include "Log.h"
#include "Neighbor.h"
#include "Vertex.h"


namespace tip
{

    template<class Elem>
    class Graph
    {
    public:
        using elem_type = Elem;
        class const_vertex_iterator; //forward declaration

    private:
        //para que puedan acceder a la implementacion de estos tipos
        friend class impl::Vertex<Graph>;
        friend class impl::Neighbor<Graph>;

        //renombre requeridos (ver class Vertex, Neighbor).
        using Vertex = impl::Vertex<Graph>; //notar el patro recursivo
        using Neighbor = impl::Neighbor<Graph>; //notar el patron recursivo.
        using Vertices = std::list<Vertex>;
        using degNeighborhood = std::list<Neighbor>;
        using Neighborhood = std::list<degNeighborhood>;
        using neighbor_iterator = typename Vertex::neighbor_iterator;
        using deg_iterator = typename Vertex::deg_iterator;


    public:

        /** CONSTRUCTORES **/
        Graph() = default;
        Graph(const Graph<Elem>&) = delete;


        /**
         * Iterador de los vertices del grafo const
         */
        class const_vertex_iterator : public std::iterator<std::bidirectional_iterator_tag,  Elem>
        {
        public:
            /**
             * Construye un iterador que no apunta a nada y es invalido.
             * Se usa simplemente para poder declarar iteradores sin definir
             */
            const_vertex_iterator();
            //constructores y destructores

            bool operator==(const_vertex_iterator other) const
            {
                return it == other.it;
            }

            bool operator!=(const_vertex_iterator other) const
            {
                return it != other.it;
            }


            const Elem& operator*() const
            {
                return it->elem;
            }

            const Elem* operator->()
            {
                return &(it->elem);
            }

            void swap(const_vertex_iterator& other)
            {
                std::swap(it, other.it);
            }

            const_vertex_iterator& operator++()
            {
                ++it;
                return *this;
            }

            const_vertex_iterator operator++(int)
            {
                const_vertex_iterator temp = *this;
                ++it;
                return temp;
            }

            const_vertex_iterator& operator--()
            {
                --it;
                return *this;
            }

            const_vertex_iterator operator--(int)
            {
                const_vertex_iterator temp = *this;
                --it;
                return temp;
            }

            neighbor_iterator begin() const
            {
                return it->begin();
            }

            neighbor_iterator cbegin() const
            {
                return begin();
            }

            neighbor_iterator end() const
            {
                return it->end();
            }
            neighbor_iterator cend() const
            {
                return end();
            }

            deg_iterator H_begin() const
            {
                return it->H_begin();
            }

            deg_iterator H_end() const
            {
                return it->H_end();
            }



        private:
            const_vertex_iterator(typename Vertices::const_iterator it) : it(it) {};
            friend class Graph;
            //para que pueda devolver el operator* un puntero al vertice en el grafo
            friend class Graph::neighbor_iterator;
            friend class Graph::deg_iterator;


            typename Vertices::const_iterator it;
        };// fin iterator vertex const

        /**
         * Inserta un nuevo vertice al grafo y retorna un iterador al elemento agregado.
         *
         * En caso de querer utilizar otro tipo de identificadores para los vertices,
         * se necesita algun tipo de diccionario externo.  En este proyecto no tomamos
         * este problema en cuenta
         *
         * @return numero del vertice agregado
         */
        const_vertex_iterator insert_vertex(const Elem& elem) {
            vertices.push_front(Vertex(elem));
            return begin();
        }



        /**
         * Elimina el vertice v del grafo si el mismo pertenece al grafo.
         * Si no pertenece no hace nada.
         *
         * @param v vertice a eliminar
         */
        void remove_vertex(const_vertex_iterator iter_v) {
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


        /**
         * Agrega la arista vw al grafo.  Si la arista ya existe, no se agrega.
         *
         * @param v uno de los vertices de la arista
         * @param w el otro vertice de la arista
         */

        void add_edge(const_vertex_iterator iter_v, const_vertex_iterator iter_w) {
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
            typename Neighborhood::iterator v_list_in_w;
            typename Neighborhood::iterator w_list_in_v;
            // ya que se asegura que v tiene grado menor o igual a w se descarta contemplar el caso contrario
            if (w->degree > v->degree) {
                auto pos_v_in_low_w = find_neighborhood_with_degree(w, v->degree+1);

                //Si estoy en highN es porque no existe una lista de ese grado aún
                if (pos_v_in_low_w == w->highNeighborhood() || impl::degree_of(pos_v_in_low_w) > v->degree+1) {

                    pos_v_in_low_w = w->insertDegNeighborhood(pos_v_in_low_w);
                }

                v_list_in_w = pos_v_in_low_w;
                w_list_in_v = v->highNeighborhood();
            }
            else {
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
        }



        /**
         * Elimina la arista vw del grafo.  Si no pertenece al grafo, no hace nada
         *
         * @param v uno de los vertices de la arista
         * @param w el otro vertice de la arista
         */
        void remove_edge(const_vertex_iterator  iter_v, const_vertex_iterator  iter_w) {
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

        /**
         * Agrega un nuevo vertice cuyo vecindario es N
         */
        const_vertex_iterator add_vertex(const Elem& elem, std::initializer_list<const_vertex_iterator> N) {
            return add_vertex(elem, N.begin(), N.end());
        }

        /**
         * Agrega un nuevo vertice con todas las aristas de [begin, end).
         * Begin y end son iteradores a un rango de const_vertex_iterator.
         */
        template<class iter>
        const_vertex_iterator add_vertex(const Elem& elem, iter begin, iter end) {
            auto v = insert_vertex(elem);
            while(begin != end) {
                add_edge(v, *begin);
                ++begin;
            }
            return v;
        }


        /**
         * @return cantidad de vertices del grafo
         */
        size_t vertexCount() const {
            return  this->vertices.size();
        };




        size_t size() const;

        /**
         *
         * @param v
         * @return grado de un vertex
         */
        size_t degree(const_vertex_iterator v) const {
            return v.it->degree;
        };


        /**
         * Retorna un iterador a los vecinos de v
         *
         * @param args
         */
        const_vertex_iterator begin() const {
            return const_vertex_iterator(vertices.begin());
        }

        const_vertex_iterator cbegin() const {
            return begin();
        }

        const_vertex_iterator end() const {
            return const_vertex_iterator(vertices.end());
        }

        const_vertex_iterator cend() const {
            return end();
        }

        deg_iterator H_begin(const_vertex_iterator v)const{
            return v.H_begin();
        }

        deg_iterator H_end(const_vertex_iterator v)const{
            return v.H_end();
        }


        neighbor_iterator N_begin(const_vertex_iterator v)const{
            return v.begin();
        }

        neighbor_iterator N_end(const_vertex_iterator v) const{
            return v.end();
        }


        /**
         * Imprime informacion de debugging
         */
        std::ostream& dump(std::ostream& out) const {
            for(auto& v : vertices) {
                out << v << std::endl;
            }
            return out;
        }



         void invariante_representacion() const {
             for(auto& vertex : vertices) {
                 vertex.invariante_representacion();
             }
         }


    private:
        Vertices vertices;


        // FUNCIONES PRIVADAS
        /**
         * Retorna un puntero a la primer lista del low_neighbhood de w que tiene grado al menos degree.
         */
        typename Neighborhood::iterator  find_neighborhood_with_degree(typename Vertices::iterator w, size_t degree)
        {
            //TODO: reemplazar por find_if
            typename Neighborhood::iterator first = w->neighborhood.begin();
            return find_neighborhood_with_degree(first, std::prev(w->neighborhood.end()), degree);
        }

        typename Neighborhood::iterator find_neighborhood_with_degree(typename Neighborhood::iterator first, typename Neighborhood::iterator last,  size_t degree)
        {
            auto it = first;
            while(it != last && it->front().neighbor->degree < degree) {
                ++it;
            }

            return it;
        }

     typename degNeighborhood::iterator find_neighbor_in(typename Neighborhood::iterator neighborhood, typename Vertices::iterator elem){
        return std::find_if(neighborhood->begin(), neighborhood->end(), [elem](Neighbor& neighbor)->bool{
        return neighbor.neighbor == elem;
        });
    }



        /**
         * APLICA LA FASE 1 AL VERTICE x.  Es decir.
         * Recorre cada w en el high neighborhood de v y actualiza la posicion de v dentro
         * del vecindario de w.  Para ello, debe pasar a v a la siguiente lista cuando
         * d(v) < d(w).  Cuando d(v) == d(w), debe bajar a N(v, d(v))
         */
        void update_neighborhood(typename Vertices::iterator x) {
            DEBUG(std::string("Graph::update_neighborhood("), x->elem, ")");
            DUMP(*this);

            for(auto it = x->highNeighborhood()->begin(); it != x->highNeighborhood()->end(); ++it){
                MESSAGE("Procesando high neighbor: ", it->neighbor->elem, "de grado", it->degree());
                if(it->degree() == x->degree) {
                    //Ya cree la lista que alberga los vertices de grado x?
                    typename Neighborhood::iterator to_list = std::prev(x->highNeighborhood());

                    if(x->highNeighborhood() == x->neighborhood.begin() || impl::degree_of(to_list) != x->degree) {
                        MESSAGE("Creando la lista de grado", x->degree);
                        to_list = x->insertDegNeighborhood(x->highNeighborhood());
                    } else {
                        to_list = std::prev(x->highNeighborhood());
                    }

                    auto to_splice = it; --it;
                    to_list->splice(to_list->begin(), *(x->highNeighborhood()), to_splice);

                    //actualizo el list_pointer  y el self_pointer del neighbor de x en w
                    auto neighobor_x = to_splice->self_pointer;// donde esta x en w

                    neighobor_x->list_pointer = to_list;

                } else {
                    it->list_pointer = it->neighbor->to_next_list(it->list_pointer, it->self_pointer);
                }
                DUMP(*this);
            }

            DEBUG(std::string("END OF Graph::update_neighborhood("), x->elem, ")");
        }


        void update_after_delete(typename Vertices::iterator x) {
            //DEBUG(std::string("BEGIN Graph::update_after_delete(") + std::to_string(x->elem) + ")");

            //segunda fase.
            for(auto it = x->highNeighborhood()->begin(); it != x->highNeighborhood()->end(); ++it){
//                DEBUG(std::string("Entrando al for Graph::update_after_delete(") + std::to_string(it->neighbor->elem) + ")");
                auto pos_x_in_w = it->neighbor->toPrevList(it->list_pointer, it->self_pointer);
                it->list_pointer = pos_x_in_w;
                it->self_pointer = pos_x_in_w->begin();
            }


            //Tercera fase
            auto prev_high = prev(x->highNeighborhood());

            if(x->highNeighborhood() != x->neighborhood.begin() and impl::degree_of(prev_high) >= x->degree){

                for(auto n = prev_high->begin(); n != prev_high->end(); ++n){
                    x->highNeighborhood()->push_front(*n);
                    auto neighbor_x = n->self_pointer; // x en w
                    //actualizamos el neighbor_x que esta en w
                    neighbor_x->list_pointer = x->highNeighborhood();
                    neighbor_x->self_pointer = x->highNeighborhood()->begin();
                    prev_high->erase(n);
                    n--;
                }

                x->neighborhood.erase(prev_high);
            }

//            DEBUG(std::string("END Graph::update_after_delete(") + std::to_string(x->elem) + ")");
            DUMP(*this);
        }






        /**
         * Esto es un hack para transformar const_iterator en iterator
         */
        typename Vertices::iterator to_iterator(const_vertex_iterator it)
        {
            return vertices.erase(it.it, it.it);
        };

        static int degree(const degNeighborhood& N) {
            return N.begin()->neighbor->degree;
        }


    };

}

template<class Elem>
std::ostream& operator<<(std::ostream& out, const tip::Graph<Elem>& G) {
    return G.dump(out);
}


#endif //GRAPH_H
