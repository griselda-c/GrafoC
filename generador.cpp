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
    char cant_vertices;
    char vertex;
    char dim;
    char vecino;


    IntGraph G;
    vector<IntGraph::const_vertex_iterator> V;


    ficheroEntrada.open ("graph.txt", ios::in);

    if (ficheroEntrada.is_open()) {

    ficheroEntrada >> cant_vertices;

    //agregamos los vertices
    for(char i = '0'; i < cant_vertices; i++) {
    ficheroEntrada >> vertex;
    int v = vertex -'0'; // convierto el char en int
    V.push_back(G.insertVertex(v));
    }

    while (! ficheroEntrada.eof() ) { //mientras no termine el archivo

        //crearmos las aristas.
        ficheroEntrada >> vertex;
        int vertice = vertex -'0';
        ficheroEntrada >> vecino;
        int vec = vecino -'0';

        if(vertex != 'F'){ // porque imprime dos veces la ultima arista
            G.add_edge(V[vertice], V[vec]);
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
