#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#include "Log.h"

using namespace std;
using namespace tip;

using IntGraph = tip::Graph<int>;


int main() {

    fstream ficheroEntrada;
    string nombre = "grafo.txt";
    string frase;

    Log::get_instance().set_output(&std::cout);
    Log::get_instance().set_level(Log::Level::NONE);

    IntGraph G;
    vector<IntGraph::const_vertex_iterator> V;

    ficheroEntrada.open(nombre.c_str(), ios::in);
    if(ficheroEntrada.is_open() ){
        while(! ficheroEntrada.eof() ){

            getline(ficheroEntrada,frase);
            int dim = frase.size() +1;
            char myArray[dim];

            strcpy(myArray, frase.c_str()); // convierte lo que recibe en char

            for(int i = 0; i < dim; i++){ //recorre el array
                int numero =  myArray[i];

                if((numero != '[' and (numero != ',' and numero != ']')) and (numero != '(' and numero != ')') ){
                   // int v = std::stoi( numero );

                   V.push_back(G.insertVertex(numero));
                }

            }


        }
    ficheroEntrada.close();
    }
    else cout<< "fichero inexistente"<<endl;



//
//    G.add_edge(V[1],V[2]);
//
//
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

    cout << G;

    cout<<G.vertexCount()<<endl;




    return 0;

}
