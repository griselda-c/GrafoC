

#include <iostream>
#include "Graph.h"
#include "Log.h"

using namespace std;
using namespace tip;

void print_vertices( Graph& G) {
    for(auto it = G.begin(); it != G.end(); ++it) {
        cout << *it << endl;
    }
}






int main() {
//     Log::get_instance().set_output(&std::cout);

    Graph G;

    auto v1 = G.insertVertex(1);
    auto v2 = G.insertVertex(2);
    auto v3 = G.insertVertex(3);
    auto v4 = G.insertVertex(4);
    auto v5 = G.insertVertex(5);
    auto v6 = G.insertVertex(6);
    auto v7 = G.insertVertex(7);
    auto v8 = G.insertVertex(8);


    G.add_edge(v1,v2);
    G.add_edge(v1,v3);
    G.add_edge(v2,v4);
//    G.add_edge(v2,v5); //ACA
//
   G.removeEdge(v1,v2);
//
   G.removeEdge(v1,v3);
//
//    G.add_edge(v3,v6);
//    G.add_edge(v3,v7);

    cout << G;

    return 0;

}
