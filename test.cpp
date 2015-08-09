

#include <iostream>
#include "Graph.h"

using namespace std;
using namespace tip;

void print_vertices( Graph& G) {
    for(auto it = G.begin(); it != G.end(); ++it) {
        cout << *it << endl;
    }
}






int main() {
    Graph G;
    auto v1 = G.insertVertex(1);
    auto v2 = G.insertVertex(2);
    auto v3 = G.insertVertex(3);
    auto v5 = G.insertVertex(4);
    auto v6 = G.insertVertex(5);
    G.add_edge(v1,v2);
    G.add_edge(v1,v3);

    G.print_vecinos(v1);
    //print_vertices(G);
//    cout << G.vertexCount() <<endl;
//    cout <<G.degree(v1) <<endl;
//    cout <<G.degree(v2) <<endl;


    return 0;
}
