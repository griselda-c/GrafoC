

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
    Log::get_instance().set_output(&std::cout);
    Log::get_instance().set_level(Log::Level::NONE);

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
    G.add_edge(v1,v7);
    G.add_edge(v1,v6);

    G.add_edge(v2,v6);

    G.add_edge(v2,v4);
    G.add_edge(v2,v5);

    G.add_vertex(9, {v3, v4, v5});

    G.removeEdge(v1,v6);
    G.remove_vertex(v6);
    G.invariante_representacion();

    G.removeEdge(v1,v3);
    G.add_edge(v3,v7);

//    cout << *G.iterHighNeighbors(v1) <<endl;
//    //que pasa si no existe
//    cout << *G.iterDegNeighborhood(v1,2) <<endl;
//
    for(auto it = v8.begin(); it != v8.end(); ++it){
        cout<< *it <<endl;
    }
//
//    for(auto z = G.H_begin(v1); z != G.H_end(v1); ++z) {
//        cout << *z << endl;
//    }

//    for(auto z = G.N_begin(v1); z != G.N_end(v1); ++z) {
//        cout << *z << endl;
//    }



    cout << G;

    return 0;

}
