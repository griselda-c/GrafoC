//#include <iostream>
//#include <vector>
//#include <string>
//#include <tuple>
//#include <cstdlib> //tiene la definicion de la funcion RAND (random)
//#include "Graph.h"
//#include "Log.h"
//#include <bits/stdc++.h>
//
//using namespace std;
//using namespace tip;
//struct Vertex {
//    Vertex(int elem) : elem(elem), marked(false) {}
//
//    int elem;
//    mutable bool marked;
//
//    void marcar(bool val) const {
//        marked = val;
//    }
//
//    bool marcado() const {
//        return marked;
//    }
//
//};
//using IntGraph = tip::Graph<Vertex>;
//
//ostream& operator<<(ostream& os, const Vertex& v) {
//    return os << v.elem;
//}
//
//ostream& operator<<(ostream& os, IntGraph::const_vertex_iterator v) {
//    return os << v->elem;
//}
//
//vector< tuple<IntGraph::const_vertex_iterator,IntGraph::const_vertex_iterator> > tuple_list;
//
//int main() {
//
//Log::get_instance().set_output(&std::cout);
//Log::get_instance().set_level(Log::Level::NONE);
//
//auto fin = 99;
//
//for(int i = 0; i <= fin; ++i){
//
//    ifstream ficheroEntrada;
//    auto archivo = "out." +std::to_string(i);
//    cout<< archivo;
//    ficheroEntrada.open (archivo, ios::in);
//    if(not ficheroEntrada.is_open()) {
//        cerr << "Fichero inexistente" << endl;
//        return 2;
//    }
//
//
//    IntGraph G;
//    vector<IntGraph::const_vertex_iterator> V;
//
//    int cant_vertices, cant_aristas;
//    ficheroEntrada >> cant_vertices >> cant_aristas;
//
//    //agregamos los vertices
//    for(int v = 0; v < cant_vertices; v++) {
//        V.push_back(G.insert_vertex(v));
//    }
//
//    for(int i = 0; i < cant_aristas; ++i)
//    {
//        int vertex, vecino;
//        //crearmos las aristas.
//        ficheroEntrada >> vertex >> vecino;
//        G.add_edge(V[vertex], V[vecino]);
//
//        //guardamos las aristas en un vector
//        auto tupla = tuple<IntGraph::const_vertex_iterator,IntGraph::const_vertex_iterator>(V[vertex],V[vecino]);
//        tuple_list.push_back(tupla);
//    }
//
//    //borramos aristas aleatoriamente
//
//    for(int i = 0; i < cant_aristas; ++i){
//        int aristas = tuple_list.size();
//        int random_arista = rand() % aristas;
//        auto tupla = tuple_list[random_arista];
////        cout << "borrando arista ";
////        cout<< get<0>(tupla)->elem<<" ," << get<1>(tupla)->elem<< endl;
//        G.removeEdge(get<0>(tupla),get<1>(tupla));
//        G.invariante_representacion();
//        tuple_list.erase(tuple_list.begin()+random_arista);
//    }
//
//    cout<<"termino " << archivo<<endl;
//
//
//    ficheroEntrada.close();
//
//}
//return 0;
//}
//
//
//
