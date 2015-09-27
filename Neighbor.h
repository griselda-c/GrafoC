
#ifndef NEIGHBOR_H
#define NEIGHBOR_H

namespace tip {
    namespace impl {
        
        /**
         * Los objetos de esta clase se van a almacenar en listas que juntas representan el vecindario N(v) de un vertice v.
         * El vecindario de N(v) se divide en dos.  Por una parte, esta la lista H(v) de los vecinos mayores o iguales a v
         * y por otra esta L(v) que es una lista L(v, 1), L(v, 2), ..., L(v, k) donde cada L(v, i) contiene los vecinos
         * de v que tienen grado i (no se guardan las listas que sean vacias).
         *
         * Cada Neighbor representa un vecino w de v que mantiene toda la informacion necesaria para
         * que sea eficiente eliminar v de N(w) cuando se tiene acceso a w en N(v).  En particular, se mantiene
         * el nombre de cada w en N(v), la ubicacion de v en la lista de N(w) que lo contiene y la ubicacion de la lista
         * de N(w) que lo contiene.
         * 
         * Esta estructura es simplemente una tripla con nombres bonitos.  Para hacerla INDEPENDIENTE del resto de los tipos
         * es que hay que establecer el tipo de Graph.  El tipo Graph debe tener los siguientes tipos internos:
         * - Graph::Vertex: es el tipo basico de un vertice del grafo.
         * - Graph::Vertices::iterator: Vertices es la coleccion que guarda vertices de tipo Graph::Vertex
         * - Graph::degNeighborhood::iterator: degNeighborhood es la coleccion que guarda los vecinos de un grado dado (o el high neighborhood).
         *                                      Por definicion, degNeighborhood::elem_type es Neighbor<Vertex>, i.e., el tipo que estamos definiendo
         *                                      En otras palabras, desreferencia un elemento de este tipo, nos da un neighbor.
         * - Graph::Neighborhood::iterator: Neighborhood es la coleccion donde se agrupan todos los degNeighborhood.
         * - Graph::Vertex::elem_type: es el tipo del elemento de Vertex.
         * 
         * Ademas, Graph::Vertex debe tener un miembro llamado elem que referencia a su elemento y otro miembro degree que referencia
         * a su grado.  En caso de no tenerlo, no invocar elem() y degree(), respectivamente.
         */
        template<class Graph>
        struct Neighbor
        {
            using NeighborPtr = typename Graph::Vertices::iterator;
            using SelfPtr = typename Graph::degNeighborhood::iterator;
            using ListPtr = typename Graph::Neighborhood::iterator;
            using elem_type = typename Graph::Vertex::elem_type;
            
            Neighbor() = default;
            Neighbor(NeighborPtr neighbor) : neighbor(neighbor) {}
            
            /**
             * Es un puntero directo al vecino en la lista de vertices.
             */
            NeighborPtr neighbor;
            
            /**
             * Es un puntero directo a la posicion de v en la lista de N(w) que lo contiene.
             */
            SelfPtr self_pointer;
            
            /**
             * Es un puntero directo a la lista de L(w) que contiene a v. (Solo tiene en cuenta los low_neighborhood;
             * cuando v esta en high_neighborhood de w, dejamos low_neighborhood.end())
             */
            ListPtr list_pointer;
            
            /**
             * Funcion auxiliar para acceder al elemento del vecino directamente
             */
            const elem_type& elem() const {
                return neighbor->elem;
            }
            elem_type& elem() {
                return neighbor->elem;
            }
            
            /**
             * Funcion auxiliar para acceder al grado del vecino directamente
             */
            size_t degree() const {
                return neighbor->degree;
            }
            
            
//             /**
//              * imprime informacion de debugging
//              */
//             std::string dump() const {
//                 std::string res;
//                 res += "vecino: " + std::to_string(neighbor->elem) + '\n';
//                 res += "self_pointer apunta a " + std::to_string(self_pointer->neighbor->elem)  + '\n';
//                 return res;
//             }
        }; //sturct Neighbor
        
    } //impl
    
}//tip

/**
 * Presupone que neighbor tiene elem bien definido y que elem admite operator <<
 */
template<class Graph>
std::ostream& operator<<(std::ostream& out, const tip::impl::Neighbor<Graph>& n) {
    return out << "(" << n.elem() << "," << n.self_pointer->elem() << ")";
}



#endif //NEIGHBOR_H