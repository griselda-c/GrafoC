

#include <iostream>
#include "Graph.h"

using namespace std;
using namespace tip;

void print_vertices(const Graph& G) {
    for(auto it = G.begin(); it != G.end(); ++it) {
      cout << *it<< endl;
    }
}

int main() {
    Graph G;
    G.insertVertex(1);
    G.insertVertex(2);
    G.insertVertex(3);
    G.insertVertex(4);
    G.insertVertex(16);

    print_vertices(G);
    cout << G.vertexCount() <<endl;
    return 0;
}
