//#include <bits/stdc++.h>
//#include "Graph.h"
//#include "Log.h"
//#include "triangles.h"
//#include <chrono>
//
//using namespace std;
//using namespace tip;
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
//
//    ofstream ficheroTiempo("denseP.ods"); // archivo que guarda registro de tiempo
//    ofstream ficheroRegistro("denseP.registro");
//
//    auto fin = 99;
//
//    for(int i = 0; i <= fin; ++i){
//
//        ifstream ficheroEntrada;
//        auto archivo = "out." +std::to_string(i);
//
//        ficheroEntrada.open (archivo, ios::in);
//
//        if(not ficheroEntrada.is_open()) {
//            cerr << "Fichero inexistente" << endl;
//            return 2;
//        }
//
//       auto start = chrono::steady_clock::now(); //inicio del reloj
//        IntGraph G;
//        vector<IntGraph::const_vertex_iterator> V;
//
//        int cant_vertices, cant_aristas;
//        ficheroEntrada >> cant_vertices >> cant_aristas;
//
//        //agregamos los vertices
//        for(int v = 0; v < cant_vertices; v++) {
//            V.push_back(G.insertVertex(v));
//        }
//
//        for(int i = 0; i < cant_aristas; ++i)
//        {
//            int vertex, vecino;
//            //crearmos las aristas.
//            ficheroEntrada >> vertex >> vecino;
//            G.add_edge(V[vertex], V[vecino]);
//        }
//
//        ficheroEntrada.close();
//
//        //busca triangulos
//        auto T = triangles(G);
//        for(auto t = T.begin(); t != T.end(); ++t) {
//            cout<< t->v << " " << t->w << " " << t->z << endl;
//        }
//        auto end = chrono::steady_clock::now();//fin del reloj
//        auto diff = end - start;
//
//        auto densidad = float(cant_aristas *2) / (G.vertexCount()*(G.vertexCount()-1));
//
//
//
//        ficheroTiempo << G.vertexCount()<<" ;" <<cant_aristas<<" ;" << chrono::duration <double, milli> (diff).count()<<";" << "ms "<<"; "<<densidad<<";"<<"densidad"<<endl;
//        ficheroRegistro<< chrono::duration <double, milli> (diff).count()<<",";
//
//    }
//
//    ficheroTiempo.close();
//    ficheroRegistro.close();
//
//    return 0;
//}
