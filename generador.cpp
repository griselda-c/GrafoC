#include <iostream>
    #include <fstream>
    #include <vector>
    #include "Graph.h"
    #include "Log.h"



    using namespace std;
    using namespace tip;

    using IntGraph = tip::Graph<int>;

    int main () {
    fstream ficheroEntrada;
    int cant_vertices;
    int vertex;
    int dim;
    int vecino;


    IntGraph G;
    vector<IntGraph::const_vertex_iterator> V;


    ficheroEntrada.open ("graph.txt", ios::in);

    if (ficheroEntrada.is_open()) {

    ficheroEntrada >> cant_vertices;

    //agregamos los vertices
    for(int i = 0; i < cant_vertices; i++) {
    ficheroEntrada >> vertex;
    V.push_back(G.insertVertex(vertex));
    }

    while (!ficheroEntrada.eof() ) { //mientras no termine el archivo

        //crearmos las aristas.
        ficheroEntrada >> vertex;

        ficheroEntrada >> vecino;

        if(vertex <= cant_vertices){ // porque imprime dos veces la ultima arista
            G.add_edge(V[vertex], V[vecino]);
        }
    }

    ficheroEntrada.close();
    }
    else cout << "Fichero inexistente" << endl;


    cout << G;

//    auto T = triangles(G);
//    for(auto t = T.begin(); t != T.end(); ++t) {
//        cout << t->v->elem << " " << t->w->elem << " " << t->z->elem << endl;
//    }


    return 0;
    }
