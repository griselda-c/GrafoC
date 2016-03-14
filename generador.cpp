//#include <bits/stdc++.h>
//#include "Graph.h"
//#include "Log.h"
//#include "triangles.h"
//#include <chrono>
//
//using namespace std;
//using namespace tip;
//
//
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
//
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
//
//
//
//int main(int argc, char*argv[]) {
//Log::get_instance().set_output(&std::cout);
//Log::get_instance().set_level(Log::Level::NONE);
//
//    if(argc < 2) {
//        cerr << "Incluir el archivo de entrada" << endl;
//        return 1;
//    }
//
//
//    ifstream ficheroEntrada(argv[1]);
//
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
//        V.push_back(G.insertVertex(v));
//    }
//
//    for(int i = 0; i < cant_aristas; ++i)
//    {
//        int vertex, vecino;
//        //crearmos las aristas.
//        ficheroEntrada >> vertex >> vecino;
//        G.add_edge(V[vertex], V[vecino]);
//    }
//
//    ficheroEntrada.close();
//    cerr << G << endl;
//
//    cout << "  [h-graph] storing the graph" << endl;
//    ofstream grafoSalida(string(argv[1]) + string(".graph"));
//    grafoSalida << cant_vertices << " " << cant_aristas << endl;
//    for(auto v = V.begin(); v != V.end(); ++v) {
//        for(auto w = v->begin(); w != v->end(); ++w) {
//            if((*v)->elem < w->elem) {
//                grafoSalida << *v << " " << w << endl;
//            }
//        }
//    }
//
//    cout << "  [h-graph] searching for triangles" << endl;
//
//    ofstream ficheroSalida(string(argv[1]) + string(".triangles"));
//
//    auto T = triangles(G);
//    for(auto t = T.begin(); t != T.end(); ++t) {
//        ficheroSalida << t->v << " " << t->w << " " << t->z << endl;
//    }
//
//    ficheroSalida.close();
//
//    return 0;
//
//}
