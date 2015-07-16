#include <iostream>
#include <list>
#include <iterator>

#ifndef GRAPH_H
#define GRAPH_H

namespace tip {

    class Graph {
    private:
        //forward declarations
        struct Vertex;
        using Vertices = std::list<Vertex>;
        
    public:
        /**
         * Iterador de los vertices del grafo
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
            
            bool operator==(const_vertex_iterator other) const {
                return it == other.it;
            }
            
            bool operator!=(const_vertex_iterator other) const {
                return it != other.it;
            }
            
            
            int operator*() const {
                return it->elem;
            }
            
//             T* operator->() {
//                 return it.operator->();
//             }

            void swap(const_vertex_iterator& other) {
                std::swap(it, other.it);
            }
            
            const_vertex_iterator& operator++() {
                ++it;
                return *this;
            }
            
            const_vertex_iterator operator++(int) {
                const_vertex_iterator temp = *this;
                ++it;
                return temp;
            }
            
            const_vertex_iterator& operator--() {
                --it;
                return *this;
            }
            
            const_vertex_iterator operator--(int) {
                const_vertex_iterator temp = *this;
                --it;
                return temp;
            }
            
            
        private:
            const_vertex_iterator(Vertices::const_iterator it) : it(it) {};
            friend class Graph;
            
            Vertices::const_iterator it;
        };
        
        
        /**
         * Iterador de las aristas
         */
        class const_edge_iterator {
        public:
            
        private:
        };
        
        
        /**
         * Iterador de los vecinos de un vertice
         */
        class const_neighbor_iterator {
            
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
        //G = std::move(H); 

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
//         vertex_iterator removeVertex(const_vertex_iterator v);

        /**
         * Agrega la arista vw al grafo.  Si la arista ya existe, no se agrega.
         *
         * @param v uno de los vertices de la arista
         * @param w el otro vertice de la arista
         */
//         edge_iterator addEdge(const_vertex_iterator v, const_vertex_iterator w);

    /**
            * Elimina la arista vw del grafo.  Si no pertenece al grafo, no hace nada
            *
            * @param v uno de los vertices de la arista
            * @param w el otro vertice de la arista
            */
//     void removeEdge(unsigned int v, unsigned int w);


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
//     int* degree(Vertex *v);

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
//     VertexIterator iterHighNeighbors(int v);


    /**
            * Retorna un iterador que nos permite recorrer N'(v)
            *
            * @param v
            * @return
            */
//     EdgeIterator iterEdgeNeighbors(int v);




    private:
        Vertices vertices;
  
        struct Neighbor;
        
        using Neighborhood = std::list<Neighbor>;
        
        struct Vertex{
            explicit Vertex(int elem, size_t degree = 0):
            elem(elem), degree(degree)
            {}
            
            int elem;
            size_t degree;
            Neighborhood highNeighborhood;
            std::list<Neighborhood> lowNeighborhood;
        };
        
        struct Neighbor {
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
            Neighbor(){}
            int  neighbor;
            
            /**
             * Es un puntero directo a la posicion de v en la lista de N(w) que lo contiene.
             */
            Neighbor *self_pointer;
            
            /**
             * Es un puntero directo a la lista de N(w) que contiene a v.
             */
            Neighborhood* list_pointer;
        };
        
        
    };

    
}

#endif //GRAPH_H


