    #include <iostream>
    #include <list>
    #include <iterator>
    #include <cassert>

    #include "Log.h"

    #ifndef GRAPH_H
    #define GRAPH_H



    namespace tip
    {

        namespace impl {
            /**
             * En este namespace vamos a agregar las clases auxiliares que utilizamos en el grafo.
             * La idea es poder tener ciertas funcionalidades sin afectar la lectura del grafo
             */

            struct Vertex;
            struct Neighbor;
            using degNeighborhood = std::list<Neighbor>;
            using Neighborhood = std::list<degNeighborhood>;
            using Vertices = std::list<Vertex>;


            int degree(degNeighborhood::const_iterator neighbor);
            int degree(const degNeighborhood& neighbors);
            int degree(const Neighborhood::const_iterator neigbhors);
            size_t elem(degNeighborhood::const_iterator neighbor);
            degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, int elem);



            /**
             * Vertex representa una instancia de un vertice del h-grafo
             * Tiene un elemento de algun tipo (por ahora entero), el grado y un vecindario.
             * El vecindario es una lista de listas N(v, d_1), ..., N(v, d_k), H donde cada lista N(v, d_i)
             * guarda los vecions de grado d_i y donde H guarda el resto de los vecinos.
             * La lista H siempre es la del final de neighborhood.
             */
            struct Vertex
            {
                int elem;  //elemento
                size_t degree;  //grado
                Neighborhood neighborhood;  //vecindario

                /**
                 * Crea un nuevo vertice con grado 0.  Siempre tiene una lista de high neighbors
                 */
                explicit Vertex(int elem, size_t degree = 0):
                elem(elem), degree(degree)
                {
                    //garantizamos el high neighbohood
                    neighborhood.push_back(degNeighborhood());
                }

                /**
                 * retorna un puntero al high neighborhood()
                 */
                Neighborhood::iterator highNeighborhood() {
                    return std::prev(neighborhood.end());
                }

                /**
                 * retorna un puntero al high neighborhood()
                 */
                Neighborhood::const_iterator highNeighborhood() const {
                    return std::prev(neighborhood.end());
                }

                /**
                 *  Mueve who de la lista list a la siguiente lista como si incrementara el grado en 1.
                 *  Se asume que el grado de who es el correcto en su lista, y no incrementa el grado.
                 *  Crea la lista en caso en que no exista.
                 *  Borra la lista list en caso que quede vacia.
                 *  Deja bien los punteros de this, pero no los de who.
                 *  Retorna la lista en la que who queda insertado.  Siempre se inserta al inicio, para poder
                 *  actualizar los punteros de who.
                 *
                 * Precondicion: list no es high_neighborhood
                 */
                Neighborhood::iterator toNextList(Neighborhood::iterator list, degNeighborhood::iterator who);

                /**
                 *  Mueve who de la lista list a la lista previa como si se decrementara el grado en 1.
                 *  Se asume que el grado de who es el de la lista destino, y no incrementa el grado.
                 *  Crea la lista en caso en que no exista.
                 *  Borra la lista list en caso que quede vacia.
                 *  Deja bien los punteros de this, pero no los de who.
                 *  Retorna la lista en la que who queda insertado.  Siempre se inserta al inicio, para poder
                 *  actualizar los punteros de who.
                 *
                 * Precondicion: who no queda en el high neighborhood
                 */
                Neighborhood::iterator toPrevList(Neighborhood::iterator list, degNeighborhood::iterator who) {
                DEBUG(std::string("BEGIN Graph::toPrevList(") + std::to_string(impl::elem(who)) + ")");

                    Neighborhood::iterator to_list;
                    if(list == neighborhood.begin() || impl::degree(std::prev(list)) < impl::degree(who)) {
                        MESSAGE(std::string("INSERTANDO MENSAJE"));
                        to_list = insertDegNeighborhood(list);
                    } else {
                        to_list = std::prev(list);
                    }
                    erase(list, who);
                    to_list->push_front(*who);
                DEBUG(std::string("END Graph::toPrevList(") + std::to_string(impl::elem(who)) + ")");
                    return to_list;

                }

                /**
                 * Borra who de la lista list, eliminando list si queda vacia y no es el high neighborhood
                 */
                void erase(Neighborhood::iterator list, degNeighborhood::iterator who);

                Neighborhood::iterator insertDegNeighborhood(Neighborhood::iterator pos) {
                    return neighborhood.insert(pos, degNeighborhood());
                }
                /**
                 * imprime informacion de debugging
                 */
                std::string dump() const;

                void invariante_representacion() const {
                    check_degree(); check_list_degrees();
                }

                /**
                 * retorna true si el grado es igual a la cantidad de elementos en su vecindario
                 */
                void check_degree() const {
                    int vecinos = 0;
                    for(auto& degn : neighborhood) {
                        vecinos += degn.size();
                    }
                    assert(vecinos == degree);
                }

                /**
                 * retorna true si todos en una lista tienen el mismo grado, ninguna lista es vacia,
                 * las listas estan en orden creciente y todos los de la ultima lista tienen grado mayor al de v
                 * y los anteriores menor
                 */
                void check_list_degrees() const {
                    for(auto itn = neighborhood.begin(); itn != highNeighborhood(); ++itn) {
                        assert(itn->size() != 0 &&
                            (std::next(itn) == highNeighborhood() ||
                            impl::degree(*itn) < impl::degree(*std::next(itn))) &&
                            degree > impl::degree(*itn));

                        for(auto w = itn->begin(); w != itn->end(); ++w) {
                            assert(impl::degree(w) == impl::degree(*itn));
                        }
                    }
                    for(auto w = highNeighborhood()->begin(); w != highNeighborhood()->end(); ++w)
                        assert(impl::degree(w) >= degree);
                }


                /**
                * Iterador de los vecinos de algun grado
                */
                class deg_iterator : std::iterator<std::bidirectional_iterator_tag,  int>
                {

                    bool operator==(deg_iterator other) const
                    {
                        return it == other.it;
                    }

                    bool operator!=(deg_iterator other) const
                    {
                        return it != other.it;
                    }


                    size_t operator*() const
                    {
                    return impl::elem(it);
                    }

                    //      T* operator->() {
                    //          return it.operator->();
                    //      }

                    void swap(deg_iterator & other)
                    {
                        std::swap(it, other.it);
                    }

                    deg_iterator& operator++()
                    {
                        ++it;
                        return *this;
                    }

                    deg_iterator  operator++(int)
                    {
                        deg_iterator temp = *this;
                        ++it;
                        return temp;
                    }

                    deg_iterator & operator--()
                    {
                        --it;
                        return *this;
                    }

                    deg_iterator operator--(int)
                    {
                        deg_iterator temp = *this;
                        --it;
                        return temp;
                    }


                    private:
                    degNeighborhood::const_iterator it;
                    deg_iterator (degNeighborhood::const_iterator it) : it(it) {};
                    friend class Vertex;
                };

                /**
                * Iterador de los vecinos (todos los vecinos)
                */
                class const_neighbor_iterator : std::iterator<std::bidirectional_iterator_tag,  int>
                {

                public:
                    /**
                    * Construye un iterador que no apunta a nada y es invalido.
                    * Se usa simplemente para poder declarar iteradores sin definir
                    */
                    const_neighbor_iterator();

                    //constructores y destructores

                    bool operator==(const_neighbor_iterator other) const
                    {
                        return it == other.it;
                    }

                    bool operator!=(const_neighbor_iterator other) const
                    {
                        return it != other.it;
                    }


                    int operator*() const
                    {
                        *it;
                   }

                    //      T* operator->() {
                    //          return it.operator->();
                    //      }

                    void swap(const_neighbor_iterator & other)
                    {
                        std::swap(it, other.it);
                    }

                    const_neighbor_iterator & operator++()
                    {
                       // std::cout<<" vengo de imprimir operator ++ " +std::to_string(*it)<<std::endl;
                        if(it == --list_it->end()){
                            ++list_it; //hay que cambiar de lista
                            if(list_it->empty()){ // si es el high y esta vacio
                                ++list_it; //avanzar el iterador
                            }
                            it = list_it->begin();
                        }
                        else{
                            ++it;
                        }
                        return *this;
                    }

                    const_neighbor_iterator  operator++(int)
                    {
                        const_neighbor_iterator temp = *this;
                        if(it == --list_it->end()){
                            ++list_it;
                            if(list_it->empty()){ // si es el high y esta vacio
                                list_it++;
                            }else{
                                 it = list_it->begin();
                            }
                        }
                        else{
                            ++it;
                        }
                        return temp;
                    }

                    const_neighbor_iterator & operator--()
                    {
                        --it;
                        return *this;
                    }

                    const_neighbor_iterator operator--(int)
                    {
                        const_neighbor_iterator temp = *this;
                        --it;
                        return temp;
                    }



                private:
                    const_neighbor_iterator (Neighborhood::const_iterator list_it) : list_it(list_it), it(list_it->begin()) {
                        if(list_it->empty()){
                            ++list_it;
                            it = list_it->begin();
                        }
                    };
                    friend class Vertex;
                    Neighborhood::const_iterator list_it;
                    deg_iterator it;

                };


                const_neighbor_iterator begin() const{
                    return const_neighbor_iterator (neighborhood.begin());
                }
                const_neighbor_iterator cbegin() const{
                    return begin();
                }

                const_neighbor_iterator end() const{
                    return const_neighbor_iterator (neighborhood.end());
                }
                const_neighbor_iterator cend() const{
                    return end();
                }

                const_neighbor_iterator H_begin() const{
                    return const_neighbor_iterator(highNeighborhood());
                }

                const_neighbor_iterator H_end() const{
                    Neighborhood::const_iterator high_end;

                    if(highNeighborhood()->empty()){
                        high_end = highNeighborhood();
                    }else{
                        high_end = ++highNeighborhood();
                    }
                    return const_neighbor_iterator(high_end);
                }

                const_neighbor_iterator iterDegNeighborhood(size_t d)const{
                    auto it = begin();
                    while(it != end() && *it < d) {
                            ++it;
                    }
                    return it;
                }

            };


            struct Neighbor
            {
                /**
                 * Los objetos de esta clase se van a almacenar en listas que juntas representan el vecindario N(v) de un vertice v.
                 * El vecindario de N(v) se divide en dos.  Por una parte, esta la lista H(v) de los vecinos mayores o iguales a v
                 * y por otra esta L(v) que es una lista L(v, 1), L(v, 2), ..., L(v, k) donde cada L(v, i) contiene los vecinos
                 * de v que tienen grado i (no se guardan las listas que sean vacias).
                 *
                 * Cada NeighborDescriptor representa un vecino w de v que mantiene toda la informacion necesaria para
                 * que sea eficiente eliminar v de N(w) cuando se tiene acceso a w en N(v).  En particular, se mantiene
                 * el nombre de cada w en N(v), la ubicacion de v en la lista de N(w) que lo contiene y la ubicacion de la lista
                 * de N(w) que lo contiene.
                 */
                Neighbor() = default;
                Neighbor(Vertices::iterator neighbor) : neighbor(neighbor) {}
                Vertices::iterator neighbor;

                /**
                 * Es un puntero directo a la posicion de v en la lista de N(w) que lo contiene.
                 */
                degNeighborhood::iterator self_pointer;

                /**
                 * Es un puntero directo a la lista de L(w) que contiene a v. (Solo tiene en cuenta los low_neighborhood;
                 * cuando v esta en high_neighborhood de w, dejamos low_neighborhood.end())
                 */
                Neighborhood::iterator list_pointer;

                /**
                 * imprime informacion de debugging
                 */
                std::string dump() const {
                    std::string res;
                    res += "vecino: " + std::to_string(neighbor->elem) + '\n';
                    res += "self_pointer apunta a " + std::to_string(self_pointer->neighbor->elem)  + '\n';
                    return res;
                }
            };

        }




        class Graph
        {
        private:
            //forward declarations
            using Neighbor = impl::Neighbor;
            using Vertex = impl::Vertex;
            using Vertices = impl::Vertices;
            using Neighborhood = impl::Neighborhood;
            using degNeighborhood = impl::degNeighborhood;
            using const_neighbor_iterator = impl::Vertex::const_neighbor_iterator;


        public:


            /**
             * Iterador de los vertices del grafo const
             */
            class const_vertex_iterator : std::iterator<std::bidirectional_iterator_tag,  int>
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


                int operator*() const
                {
                    return it->elem;
                }

                //      template <class T> T* operator->() {
                //          return it.operator->();
                //      }

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

                const_neighbor_iterator begin() const{
                    return it->begin();
                }
                const_neighbor_iterator cbegin() const{
                    return begin();
                }

                const_neighbor_iterator end() const{
                    return it->end();
                }
                const_neighbor_iterator cend() const{
                    return end();
                }



            private:
                const_vertex_iterator(Vertices::const_iterator it) : it(it) {};
                friend class Graph;

                Vertices::const_iterator it;
            };// fin iterator vertex const



            /**
             * Iterador de las aristas
             */
            class const_edge_iterator
            {
            public:

            private:
            };

            //Constructor por defecto
            //Graph() { }

            //Destructor.  Libera todos los recursos.
            //~Graph() {}

            //Constructor por copia
            //Graph(const Graph& other);
            //Graph H; //constructor por defecto
            //Graph G(H); constructor por copia
            //Graph G{H}; constructor por copia
            //Graph G = H; constructor por copia.

            //operador de asignacion
            //Graph& operator=(const Graph& graph);
            //G = H;

            //Move constructor
            //Graph(Graph&& other);
            //Graph G = f() donde f() retorna un grafo por copia (evita la copia)

            //Asignacion por movimiento
            //Graph& operator=(Graph&& other)
            //G = f() donde f() retorna un grafo por copia (evita la copia)
            //G = std::move(H

            /**
             * Inserta un nuevo vertice al grafo y retorna el numero del indice agregado.
             *
             * En caso de querer utilizar otro tipo de identificadores para los vertices,
             * se necesita algun tipo de diccionario externo.  En este proyecto no tomamos
             * este problema en cuenta
             *
             * @return numero del vertice agregado
             */
            const_vertex_iterator insertVertex(unsigned int elem);

            /**
             * Elimina el vertice v del grafo si el mismo pertenece al grafo.
             * Si no pertenece no hace nada.
             *
             * @param v vertice a eliminar
             */
            //   vertex_iterator removeVertex(const_vertex_iterator v);
            void remove_vertex(const_vertex_iterator v);

            /**
             * Agrega la arista vw al grafo.  Si la arista ya existe, no se agrega.
             *
             * @param v uno de los vertices de la arista
             * @param w el otro vertice de la arista
             */
            //  edge_iterator addEdge(const_vertex_iterator v, const_vertex_iterator w);
            void add_edge(const_vertex_iterator v, const_vertex_iterator w);

            /**
             * Elimina la arista vw del grafo.  Si no pertenece al grafo, no hace nada
             *
             * @param v uno de los vertices de la arista
             * @param w el otro vertice de la arista
             */
            void removeEdge(const_vertex_iterator  v, const_vertex_iterator  w);

            /**
             * Agrega un nuevo vertice cuyo vecindario es N
             */
            const_vertex_iterator add_vertex(unsigned int elem, std::initializer_list<const_vertex_iterator> N) {
                return add_vertex(elem, N.begin(), N.end());
            }

            /**
             * Agrega un nuevo vertice con todas las aristas de [begin, end).
             * Begin y end son iteradores a un rango de const_vertex_iterator.
             */
            template<class iter>
            const_vertex_iterator add_vertex(unsigned int elem, iter begin, iter end) {
                auto v = insertVertex(elem);
                while(begin != end) {
                    add_edge(v, *begin);
                    ++begin;
                }
                return v;
            }


            /**
             * @return cantidad de vertices del grafo
             */
            int vertexCount() const;


            int size() const;

            /**
             *
             * @param v
             * @return grado de un vertex
             */
            int degree(const_vertex_iterator v) const;


            /**
             * Pertenencia de la arista vw
             *
             * @param v
             * @param w
             * @return true si la arista pertenece al grafo.
             */
            const_edge_iterator find_edge(const_vertex_iterator v, const_vertex_iterator w) const;

            /**
             * Retorna un iterador a los vecinos de v
             *
             * @param args
             */
            const_vertex_iterator begin() const;
            const_vertex_iterator cbegin() const;

            const_vertex_iterator end() const;
            const_vertex_iterator cend() const;

            const_neighbor_iterator H_begin(const_vertex_iterator v) const;
            const_neighbor_iterator H_end(const_vertex_iterator v) const;

//            const_neighbor_iterator N_begin() const;
//            const_neighbor_iterator N_end() const;
//
//            const_degree_iterator H_begin() const;
//            const_degree_iterator H_end() const;


            /**
             * Retorna un iterador a los vecinos de v que tienen grado al menos d(v)
             *
             * @param v
             * @return iterador a los vecinos de v.
             */
            const_neighbor_iterator iterHighNeighbors(const_vertex_iterator v);

            const_neighbor_iterator iterDegNeighborhood(const_vertex_iterator v,size_t degree) const;


            /**
             * Retorna un iterador que nos permite recorrer N'(v)
             *
             * @param v
             * @return
             */
            //  EdgeIterator iterEdgeNeighbors(int v);

            //void print_vecinos(const_vertex_iterator x) const;

            /**
             * Imprime informacion de debugging
             */
            std::ostream& dump(std::ostream& out) const;

             void invariante_representacion() const {
                 for(auto& vertex : vertices) {
                     vertex.invariante_representacion();
                 }
             }


        private:
            Vertices vertices;


            // FUNCIONES PRIVADAS

            std::list<degNeighborhood>::iterator  find_neighborhood_with_degree(Vertices::iterator w, int degree);
            std::list<degNeighborhood>::iterator  find_neighborhood_with_degree(std::list<degNeighborhood>::iterator first, std::list<degNeighborhood>::iterator last,  int degree);
            void update_neighborhood(Vertices::iterator x);
            void update_after_delete(Vertices::iterator x);



            /**
             * Esto es un hack para transformar const_iterator en iterator
             */
            Vertices::iterator to_iterator(const_vertex_iterator it)
            {
                return vertices.erase(it.it, it.it);
            };

            static int degree(const degNeighborhood& N) {
                return N.begin()->neighbor->degree;
            }

        };

    }

    std::ostream& operator<<(std::ostream& out, const tip::impl::Vertex& v);
    std::ostream& operator<<(std::ostream& out, const tip::impl::Neighbor& v);
    std::ostream& operator<<(std::ostream& out, const tip::Graph& G);
    #endif //GRAPH_H

