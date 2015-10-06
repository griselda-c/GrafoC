
#ifndef VERTEX_H
#define VERTEX_H

#include <list>
#include "Log.h"
//#include "Neighbor.h"

namespace tip {

    namespace impl {
        /**
         * Vertex representa una instancia de un vertice del h-grafo.
         * Tiene un elemento de algun tipo (copiable, porque se guarda una copia), el grado y un vecindario.
         * El vecindario es una lista de listas N(v, d_1), ..., N(v, d_k), H donde cada lista N(v, d_i)
         * guarda los vecions de grado d_i y donde H guarda el resto de los vecinos.
         * La lista H siempre es la del final de neighborhood.
         *
         * Para guardar los vecinos se usa la estructura de soporte Neighbor<Graph>.  Esta es una estructura
         * que depende de Graph.  A su vez, Vertex tambien depende de graph porque necesita guardar sus vecinos de
         * alguna forma.  Se supone que Graph tiene las siguiente funciones/tipos (ver tambien Neighbor):
         * - Graph::Vertex: es el tipo basico de un vertice del grafo, i.e., Vertex<Graph> (notar el patro recursivo).
         * - Graph::Vertices::iterator: Vertices es la coleccion que guarda vertices de tipo Graph::Vertex
         * - Graph::degNeighborhood::iterator: degNeighborhood es la coleccion que guarda los vecinos de un grado dado (o el high neighborhood).
         *                                      Por definicion, degNeighborhood::elem_type es Neighbor<Vertex>, i.e., el tipo que estamos definiendo
         *                                      En otras palabras, desreferencia un elemento de este tipo, nos da un neighbor.
         * - Graph::Neighborhood::iterator: Neighborhood es la coleccion donde se agrupan todos los degNeighborhood.
         * - Graph::elem_type: es el tipo del elemento de Vertex.
         * Se supone que Vertices, degNeighborhood y Neighborhood tienen la misma interfaz que std::list.
         *
         * Esta estructura es de soporte, por lo cual muestra su implementacion interana.  Se asume que
         * funciona dentro de la estructura h-grafo.
         */
        template<class Graph>
        struct Vertex
        {
            //Tipo del elemento que se guarda en el vertice
            using elem_type = typename Graph::elem_type;
            //Estructura donde se guardan los vertices
            using Vertices = typename Graph::Vertices;
            //Estructura de vecino.
            using degNeighborhood = typename Graph::degNeighborhood;
            //Estructura donde se guardan todos los vecindarios de todos los grados.
            using Neighborhood = typename Graph::Neighborhood;

            elem_type elem;  //elemento
            size_t degree;  //grado
            Neighborhood neighborhood;  //vecindario

            /**
             * Crea un nuevo vertice con grado 0.  Siempre tiene una lista de high neighbors.
             * Costo: O(copiar elem)
             */
            explicit Vertex(const elem_type& elem, size_t degree = 0):
            elem(elem), degree(degree)
            {
                //garantizamos el high neighbohood
                neighborhood.push_back(degNeighborhood());
            }

            /**
             * retorna un puntero al high neighborhood()
             */
            typename Neighborhood::iterator highNeighborhood()
            {
                return std::prev(neighborhood.end());
            }

            /**
             * retorna un puntero al high neighborhood()
             */
            typename Neighborhood::const_iterator highNeighborhood() const
            {
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
            typename Neighborhood::iterator toNextList(typename Neighborhood::iterator list, typename degNeighborhood::iterator who)
            {
                //DEBUG(this->elem, "-> Graph::toNextList(", *who, ")");
                assert(list != highNeighborhood());
                auto to_list = std::next(list);

                if(who->degree() + 1 < this->degree and
                    (to_list == highNeighborhood() || degree_of(to_list) != who->degree()+1))
                {
                    //MESSAGE("Creando la lista de grado", impl::degree(who), +"para albergar a", *who);
                    to_list = insertDegNeighborhood(to_list);
                }
                to_list->push_front(*who);
                MESSAGE("Borrando a", *who, "de su lista");
                erase(list, who);
                DEBUG("END OF Graph::toNextList");
                return to_list;
            }


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
            typename Neighborhood::iterator toPrevList(typename Neighborhood::iterator list, typename degNeighborhood::iterator who) {
                DEBUG("BEGIN Graph::toPrevList(", *who, ")");

                typename Neighborhood::iterator to_list;
                if(list == neighborhood.begin() || degree_of(std::prev(list)) < who->degree())
                {
                    MESSAGE(std::string("INSERTANDO MENSAJE"));
                    to_list = insertDegNeighborhood(list);
                } else {
                    to_list = std::prev(list);
                }
                erase(list, who);
                to_list->push_front(*who);
                DEBUG("END Graph::toPrevList(", *who, ")");
                return to_list;

            }

            /**
             * Borra who de la lista list, eliminando list si queda vacia y no es el high neighborhood
             */
            void erase(typename Neighborhood::iterator list, typename degNeighborhood::iterator who) {
                DEBUG(*this, "-> Graph::erase(", *who, ")");
                list->erase(who);
                if(list->empty() && list != highNeighborhood()) {
                    MESSAGE("Borrando la lista que quedo vacia");
                    neighborhood.erase(list);
                }
                DEBUG("END OF Graph::erase");
            }

            typename Neighborhood::iterator insertDegNeighborhood(typename Neighborhood::iterator pos) {
                return neighborhood.insert(pos, degNeighborhood());
            }

            /**
             * Iterador de los vecinos de algun grado
             */
            class deg_iterator : public std::iterator<std::bidirectional_iterator_tag, elem_type>
            {
            public:

                bool operator==(deg_iterator other) const
                {
                    return it == other.it;
                }

                bool operator!=(deg_iterator other) const
                {
                    return it != other.it;
                }


                const elem_type& operator*() const
                {
                    return it->neighbor->elem; //elem(it);
                }

                const elem_type* operator->() {
                    return it.operator->();
                }

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
                typename degNeighborhood::const_iterator it;
                deg_iterator (typename degNeighborhood::const_iterator it) : it(it) {};
                friend class Vertex;
            }; //class deg_iterator

            /**
             * Iterador de los vecinos (todos los vecinos)
             */
            class neighbor_iterator : public std::iterator<std::bidirectional_iterator_tag,  elem_type>
            {

            public:
                /**
                 * Construye un iterador que no apunta a nada y es invalido.
                 * Se usa simplemente para poder declarar iteradores sin definir
                 */
                neighbor_iterator();

                //constructores y destructores

                bool operator==(neighbor_iterator other) const
                {
                    return it == other.it; //and list_it == other.list_it (innecesario)
                }

                bool operator!=(neighbor_iterator other) const
                {
                    return it != other.it;
                }


                const elem_type& operator*() const
                {
                    *it;
                }

                const elem_type* operator->() {
                    return it.operator->();
                }

                void swap(neighbor_iterator & other)
                {
                    std::swap(list_it, other.list_it);
                    std::swap(it, other.it);
                }

                neighbor_iterator & operator++()
                {
                    ++it;
                    if(it == list_it->end() and list_it != high){
                        ++list_it;
                        it = list_it->begin();
                    }
                    return *this;
                }

                neighbor_iterator  operator++(int)
                {
                    auto temp = *this;
                    ++it;
                    return temp;
                }

                neighbor_iterator & operator--()
                {
                    if((list_it == high and high->empty()) or it == list_it->begin()) {
                        --list_it;
                        it = std::prev(list_it->end());
                    } else {
                        --it;
                    }
                    return *this;
                }

                neighbor_iterator operator--(int)
                {
                    auto temp = *this;
                    --it;
                    return temp;
                }

            private:
                neighbor_iterator (typename Neighborhood::const_iterator list_it, deg_iterator it, typename Neighborhood::const_iterator highNeighborhood) : list_it(list_it), it(it), high(highNeighborhood)
                {};

                /** Vamos a suponer que la posicion end se corresponde a:
                 * list_it apunta a high,
                 * it apunta a high->end()
                 */
                friend class Vertex;
                typename Neighborhood::const_iterator list_it;
                typename Neighborhood::const_iterator high; //para saber si list_it apunta a high
                deg_iterator it;

            }; //class neighbor_iterator


            neighbor_iterator begin() const{
                auto first_list = neighborhood.begin();
                auto first_elem = deg_iterator(first_list->begin());
                return neighbor_iterator (first_list, first_elem, highNeighborhood());
            }
            neighbor_iterator cbegin() const{
                return begin();
            }

            neighbor_iterator end() const{
                return neighbor_iterator (highNeighborhood(), H_end(), highNeighborhood());
            }
            neighbor_iterator cend() const{
                return end();
            }

            deg_iterator H_begin() const{
                return deg_iterator(highNeighborhood()->begin());
            }

            deg_iterator H_end() const{
                return deg_iterator(highNeighborhood()->end());
            }


//             /**
//              * imprime informacion de debugging
//              */
//             std::string dump() const {
//                 {
//                     std::string res;
//                     res += "elemento: " + std::to_string(elem) + '\n';
//                     res += "grado: " + std::to_string(degree) + '\n';
//                     for(auto it_degn = neighborhood.begin(); it_degn != neighborhood.end(); ++it_degn) {
//                         if(it_degn != highNeighborhood()) {
//                             res += "  Vecinos de grado: " + std::to_string(impl::degree(it_degn)) + '\n';
//                         } else {
//                             res += "  High neighborhood\n";
//                         }
//                         for(auto& neighbor : *it_degn) {
//                             res += "    neighborhood: " + neighbor.dump();
//                         }
//                         res += '\n';
//                     }
//                     return res;
//                 }
//             }
//
//
//             void invariante_representacion() const {
//                 check_degree(); check_list_degrees();
//             }
//
//             /**
//              * retorna true si el grado es igual a la cantidad de elementos en su vecindario
//              */
//             void check_degree() const {
//                 int vecinos = 0;
//                 for(auto& degn : neighborhood) {
//                     vecinos += degn.size();
//                 }
//                 assert(vecinos == degree);
//             }
//
//             /**
//              * retorna true si todos en una lista tienen el mismo grado, ninguna lista es vacia,
//              * las listas estan en orden creciente y todos los de la ultima lista tienen grado mayor al de v
//              * y los anteriores menor
//              */
//             void check_list_degrees() const {
//                 for(auto itn = neighborhood.begin(); itn != highNeighborhood(); ++itn) {
//                     assert(itn->size() != 0 &&
//                     (std::next(itn) == highNeighborhood() ||
//                     impl::degree(*itn) < impl::degree(*std::next(itn))) &&
//                     degree > impl::degree(*itn));
//
//                     for(auto w = itn->begin(); w != itn->end(); ++w) {
//                         assert(impl::degree(w) == impl::degree(*itn));
//                     }
//                 }
//                 for(auto w = highNeighborhood()->begin(); w != highNeighborhood()->end(); ++w)
//                     assert(impl::degree(w) >= degree);
//             }

        }; //class Vertex


        //FUNCIONES AUXILIARES PARA ACCEDER AL GRADO DE UNA LISTA DE VECINOS
//         template<class Graph>
//         size_t degree(typename degNeighborhood<Elem>::const_iterator neighbor) {
//             return neighbor->neighbor->degree;
//         }

        //Dada cualquier colecion de Neighbors (cosas que entienden la funcion degree), vamos a tomar que
        //el grado de la coleccion es el del primer neighbor.  La idea es usarlo para saber el grado de un degNeighborhood.
        //Esta funcion, en particular, toma un puntero a una coleccion de Neighbors.  La idea es poder usar un iterator
        //de Neighborhood, que es una coleccion de degNeighborhood, que adentro contiene sus Neighbors.
        //Se asume que *neighbors es una coleccion y por lo tanto tiene begin y end, y que los elementos
        //de *neighbors acceptan la funcion degree.
        template<class NeighborhoodIterator>
        size_t degree_of(NeighborhoodIterator neighbors) {
            assert(neighbors->begin() != neighbors->end());
            return neighbors->begin()->degree();
        }


    } //namespace impl
} //namespace tip

template<class Graph>
std::ostream& operator<<(std::ostream& out, const tip::impl::Vertex<Graph>& v)
{
    out << '{' << v.elem << ", " << v.degree << '}';
    out << '<';
    for(auto it_degn = v.neighborhood.begin(); it_degn != v.neighborhood.end(); ++it_degn) {
        if(it_degn != v.highNeighborhood()) {
            out << "N(v," << tip::impl::degree_of(it_degn) << "): [";
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



#endif //VERTEX_H
