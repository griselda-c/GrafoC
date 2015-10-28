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
    V.push_back(G.insertVertex( (int) vertex));
    //V[(int) vertex] = G.insertVertex((int) vertex);

    }

while (! ficheroEntrada.eof() ) {

// crearmos las aristas.
 ficheroEntrada >> dim;
 ficheroEntrada >> vertex;
 cout <<" vertice "<< vertex;
 for(char j = '0'; j < dim; j++){

    ficheroEntrada >> vecino;

    cout << " vecino " <<vecino;

 }

 cout <<"" <<endl;

}
ficheroEntrada.close();
}
else cout << "Fichero inexistente" << endl;

cout << G;
return 0;
}
