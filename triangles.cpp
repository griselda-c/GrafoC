

#include <list>
#include <vector>
#include "Graph.h"

using namespace tip;
using namespace std;


struct Vertex {
    Vertex(int elem) : elem(elem), marked(false) {}

    int elem;
    mutable bool marked;
    
    void marcar(bool val) const {
        marked = val;
    }
    
    bool marcado() const {
        return marked;
    }
};

ostream& operator<<(ostream& os, const Vertex& v) {
    return os << v.elem;
}

using Grafo = Graph<Vertex>

int main() {

   Grafo G;

   vector<Grafo::const_vertex_iterator> V;

   V.push_back(G.add_vertex(Vertex(0), {}));
   V.push_back(G.add_vertex(Vertex(1), {V[0]}));
   V.push_back(G.add_vertex(Vertex(2), {V[0], V[1]}));

   auto T = triangles(G);
   for(auto t = T.begin(); t != T.end(); ++t) {
       cout << t->v->elem << " " << t->w->elem << " " << t->z->elem << endl;
   }

}
