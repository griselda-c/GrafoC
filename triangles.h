
#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <list>
#include "Graph.h"

namespace tip {

    
    template<class T>
    struct Triangle {
        typename Graph<T>::const_vertex_iterator v, w, z;
    };

    template<class T>
    bool less(const Graph<T>& G, typename Graph<T>::const_vertex_iterator v, typename Graph<T>::const_vertex_iterator w) {
        return G.degree(v) < G.degree(w) || (G.degree(v) == G.degree(w) && v->elem < w->elem);
    }

    template<class T>
    bool well_sorted(const Graph<T>& G, Triangle<T> t) {
        return less(G, t.v, t.w) && less(G, t.w, t.z);
    }

    //Marcable representa cualquier objeto que tenga los metodos:
    // - o.marcar(bool) que dice si la marca esta prendida (true) o apagada (false)
    // - o.marcado() que retorna el valor de la marca
    //Estos metodos DEBEN funcionar cuando o es constante; asi que lo que modifiquen sera mutable
    template<class Marcable>
    std::list<Triangle<Marcable>> triangles(const Graph<Marcable>& G) {
        std::list<Triangle<Marcable>> res;

        for(auto v = G.begin(); v != G.end(); ++v) {
            for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
                w->marcar(true);
            }

            //N_begin retorna neighbor_iterator
            //G.H_begin necesita un const_vertex_iterator.
            for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
                for(auto z = G.H_begin(w); z != G.H_end(w); ++z) {
                    if(z->marcado() && well_sorted(G, Triangle<Marcable>{v,w,z}))
                        res.push_back(Triangle<Marcable>{v, w, z});
                }
            }

            for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
                w->marcar(false);
            }

        }

        return res;
    }

};

#endif //TRIANGLES_H