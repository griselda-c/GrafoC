

#include "Graph.h"

using namespace tip;
using namespace std;


template<typename T>
struct Triangle {
 //typename Graph<T>::const_vertex_iterator v, w, z;
  typename Graph<T>::neighbor_iterator v, w, z;

};

struct Vertex {
    int elem;
    mutable bool marked;
};

list<Triangle<Vertex>>triangles(const Graph<Vertex> G) {
    list<Triangle<Vertex>> res;

    for(auto v = G.begin(); v != G.end(); ++v) {
        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            w->marked = true;
        }

        //N_begin retorna neighbor_iterator
        //G.H_begin necesita un const_vertex_iterator.
        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            for(auto z = G.H_begin(w); z != G.H_end(w); ++z) {
                if(z->marked)
                    res.push_back(Triangle<Vertex>{v, w, z});
            }
        }

        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            w->marked = false;
        }

    }

    return res;
}
