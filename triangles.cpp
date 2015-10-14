

#include <list>
#include <vector>
#include "Graph.h"

using namespace tip;
using namespace std;

struct Vertex {
    Vertex(int elem) : elem(elem), marked(false) {}

    int elem;
    mutable bool marked;
};

ostream& operator<<(ostream& os, const Vertex& v) {
    return os << v.elem;
}

using Grafo = tip::Graph<Vertex>;

// struct Triangle {
//   typename Graph<Vertex>::const_vertex_iterator v, w, z;
//   Grafo::neighbor_iterator v, w, z;
// };

struct Triangle {
  Grafo::const_vertex_iterator v, w, z;
};

bool less(const Grafo& G, Grafo::const_vertex_iterator v, Grafo::const_vertex_iterator w) {
    return G.degree(v) < G.degree(w) || (G.degree(v) == G.degree(w) && v->elem < w->elem);
}

bool well_sorted(const Grafo& G, Triangle t) {
    return ::less(G, t.v, t.w) && ::less(G, t.w, t.z);
}

list<Triangle> triangles(const Grafo G) {
    list<Triangle> res;

    for(auto v = G.begin(); v != G.end(); ++v) {
        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            w->marked = true;
        }

        //N_begin retorna neighbor_iterator
        //G.H_begin necesita un const_vertex_iterator.
        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            for(auto z = G.H_begin(w); z != G.H_end(w); ++z) {
                if(z->marked && well_sorted(G, Triangle{v,w,z}))
                    res.push_back(Triangle{v, w, z});
            }
        }

        for(auto w = G.N_begin(v); w != G.N_end(v); ++w) {
            w->marked = false;
        }

    }

    return res;
}

//int main() {
//
//    Grafo G;
//
//    vector<Grafo::const_vertex_iterator> V;
//
//    V.push_back(G.add_vertex(Vertex(0), {}));
//    V.push_back(G.add_vertex(Vertex(1), {V[0]}));
//    V.push_back(G.add_vertex(Vertex(2), {V[0], V[1]}));
//
//    auto T = triangles(G);
//    for(auto t = T.begin(); t != T.end(); ++t) {
//        cout << t->v->elem << " " << t->w->elem << " " << t->z->elem << endl;
//    }
//
//}
