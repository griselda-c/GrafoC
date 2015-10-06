

#include <iostream>
#include <vector>
#include "Graph.h"
#include "Log.h"

using namespace std;
using namespace tip;

using IntGraph = tip::Graph<int>;

void print_vertices( IntGraph& G) {
    for(auto it = G.begin(); it != G.end(); ++it) {
        cout << *it << endl;
    }
}

/*
namespace std {
template<>
struct iterator_traits<IntGraph::deg_iterator> {
   using difference_type = IntGraph::deg_iterator::difference_type; //ptrdiff_t;
   using value_type = size_t;
   using pointer = const size_t*;
   using reference = const size_t&;
   using iterator_category = std::bidirectional_iterator_tag;
};
}
*/

int main() {
    Log::get_instance().set_output(&std::cout);
    Log::get_instance().set_level(Log::Level::NONE);

    IntGraph G;
    vector<IntGraph::const_vertex_iterator> V;

    for(int i = 0; i < 9; ++i) {
        V.push_back(G.insertVertex(i));
    }
/*
    auto V[1] = G.insertVertex(1);
    auto V[2] = G.insertVertex(2);
    auto V[3] = G.insertVertex(3);
    auto V[4] = G.insertVertex(4);
    auto V[5] = G.insertVertex(5);
    auto V[6] = G.insertVertex(6);
    auto V[7] = G.insertVertex(7);
    auto V[8] = G.insertVertex(8);
*/
//    G.add_edge(V[1],V[2]);
//    G.add_edge(V[1],V[3]);
//    G.add_edge(V[1],V[7]);
//    G.add_edge(V[1],V[6]);
//
//    G.add_edge(V[2],V[6]);
//
//    G.add_edge(V[2],V[4]);
//    G.add_edge(V[2],V[5]);
//
//    V.push_back(G.add_vertex(9, {V[3], V[4], V[5]}));
//
////    G.removeEdge(V[1],V[6]);
////    G.remove_vertex(V[6]);
////     G.invariante_representacion();
//
////    G.removeEdge(V[1],V[3]);
////    G.add_edge(V[3],V[7]);
//
//
////    for(auto it = V[2].begin(); it != V[2].end(); ++it){
////        cout<< *it <<endl;
////    }
////
//    G.dump(cout);
//    for(auto& v : V) {
//        cout << "recorriendo H(" << *v << "): ";
//        for(auto z = G.H_begin(v); z != G.H_end(v); ++z) {
//            cout << *z << ',';
//        }
//        cout << endl;
//        cout << "recorriendo H(" << *v << ") al reves: ";
//        for(auto z = G.H_end(v); z != G.H_begin(v); --z) {
//            cout << *prev(z) << ',';
//        }
//        cout << endl;
//    }
////    auto x = G.H_end(V[1]);
////    --x;
////    cout << *x << endl;
////    --x;
////    cout << *x << endl;
////    --x;
////    cout << *x << endl;
////    --x;
////    cout << *x << endl;
//
//
//
//    for(auto& v : V) {
//        cout << "recorriendo N(" << *v << "): ";
//        auto z = G.N_begin(v);
//        for(; z != G.N_end(v); ++z) {
//            cout << *z << ',';
//        }
//        cout << endl;
//        cout << "recorriendo N(" << *v << ") al reves: ";
//        for(; z != G.N_begin(v); --z) {
//            cout << *prev(z) << ',';
//        }
//        cout << endl;
//    }
//
//
//    cout << G;

    print_vertices(G);


    return 0;

}
