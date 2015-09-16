

#include "Graph.h"

using namespace tip;
using namespace std;

struct Triangle {
    Graph::const_vertex_iterator v, w, z;
};

struct Vertex {
    int elem;
    volatile bool marked;
};

//list<Triangle> triangles(const Graph<Vertex> G) {
//    list<Triangle> res;
//
//    for(auto v = G.begin(); v != G.end(); ++v) {
//        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
//            w->marked = true;
//        }
//
//        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
//            for(auto z = G.H_begin(w); z != G.H_end(w); ++z) {
//                if(z->marked)
//                    res.push_back(Triangle{v, w, z});
//            }
//        }
//
//        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
//            w->marked = false;
//        }
//
//    }
//
//    return res;
//}
