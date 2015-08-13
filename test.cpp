

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

    G.add_edge(v1,v2);
    G.add_edge(v4,v5);

    G.add_edge(v2,v4);

    cout << G;
    return 0;

}
