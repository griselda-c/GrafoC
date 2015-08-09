#include <iostream>
#include <list>
#include <iterator>

#ifndef GRAPH_H
#define GRAPH_H



namespace tip
{

    namespace impl {
        /**
         * En este namespace vamos a agregar las clases auxiliares que utilizamos en el grafo.
         * La idea es poder tener ciertas funcionalidades sin afectar la lectura del grafo
         */

        struct Neighbor;
        struct Vertex;
        using degNeighborhood = std::list<Neighbor>;
        using Neighborhood = std::list<degNeighborhood>;
        using Vertices = std::list<Vertex>;


        int degree(degNeighborhood::const_iterator neighbor);
        int degree(const degNeighborhood& neighbors);
        int degree(const Neighborhood::const_iterator neigbhors);
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
            Neighborhood::iterator toNextList(Neighborhood::iterator list, degNeighborhood::iterator who) {
                auto to_list = std::next(list);
                if( impl::degree(who) < this->degree && impl::degree(to_list) != impl::degree(who)) {
                    to_list = insertDegNeighborhood(to_list);
                }
                to_list->push_front(*who);
                erase(list, who);
                return to_list;
            }

            /**
             *  Mueve who de la lista list a la lista previa como si se decrementara el grado en 1.
             *  Se asume que el grado de who el de la lista destino, y no incrementa el grado.
             *  Crea la lista en caso en que no exista.
             *  Borra la lista list en caso que quede vacia.
             *  Deja bien los punteros de this, pero no los de who.
             *  Retorna la lista en la que who queda insertado.  Siempre se inserta al inicio, para poder
             *  actualizar los punteros de who.
             *
             * Precondicion: who no queda en el high neighborhood
             */
            Neighborhood::iterator toPrevList(Neighborhood::iterator list, degNeighborhood::iterator who) {
                Neighborhood::iterator to_list;
                if(list == neighborhood.begin() || impl::degree(std::prev(list)) < impl::degree(who)) {
                    to_list = insertDegNeighborhood(list);
                } else {
                    to_list = std::prev(list);
                }
                erase(list, who);
                to_list->push_front(*who);
                return to_list;
            }

            /**
             * Borra who de la lista list, eliminando list si queda vacia y no es el high neighborhood
             */
            void erase(Neighborhood::iterator list, degNeighborhood::iterator who) {
                list->erase(who);
                if(list->empty() && list != highNeighborhood()) {
                    neighborhood.erase(list);
                }
            }

            Neighborhood::iterator insertDegNeighborhood(Neighborhood::iterator pos) {
                return neighborhood.insert(pos, degNeighborhood());
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



        /**
         * Iterador de los vecinos
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


            Vertices::iterator operator*() const
            {
                return it->neighbor;
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
                ++it;
                return *this;
            }

            const_neighbor_iterator  operator++(int)
            {
                const_neighbor_iterator temp = *this;
                ++it;
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
            const_neighbor_iterator (std::list<Neighbor>::const_iterator it) : it(it) {};
            friend class Graph;
            std::list<Neighbor>::const_iterator it;
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



        /**
         * Retorna un iterador a los vecinos de v que tienen grado al menos d(v)
         *
         * @param v
         * @return iterador a los vecinos de v.
         */
        //  VertexIterator iterHighNeighbors(int v);


        /**
         * Retorna un iterador que nos permite recorrer N'(v)
         *
         * @param v
         * @return
         */
        //  EdgeIterator iterEdgeNeighbors(int v);

        void print_vecinos(const_vertex_iterator x);


    private:
        Vertices vertices;


        // FUNCIONES PRIVADAS

        std::list<degNeighborhood>::iterator  find_neighborhood_with_degree(Vertices::iterator w, int degree);
        std::list<degNeighborhood>::iterator  find_neighborhood_with_degree(std::list<degNeighborhood>::iterator first, std::list<degNeighborhood>::iterator last,  int degree);
//         degNeighborhood::iterator find_neighbor_in(Neighborhood::iterator neighborhood, int elem);

        void remove_edge(Vertices::iterator v, Vertices::iterator w);
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

#endif //GRAPH_H


