

#include <iostream>
#include "Graph.h"

using namespace std;
using namespace tip;

void print_vertices( Graph& G) {
    for(auto it = G.begin(); it != G.end(); ++it) {
      G.cambiarDegree(it);
     cout << G.degree(it)<< endl;
    }
}

int main() {
    Graph G;
   auto v1 =  G.insertVertex(1);
   G.cambiarDegree(v1);
   auto v2 =  G.insertVertex(2);
    G.insertVertex(3);
    G.insertVertex(4);
    G.insertVertex(16);

//    print_vertices(G);
//    cout << G.vertexCount() <<endl;
    cout <<G.degree(v1) <<endl;
    cout <<G.degree(v2) <<endl;
    cout <<G.is_greater_degree(v1,v2) <<endl;
    return 0;
}
