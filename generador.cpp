#include <bits/stdc++.h>
#include "Graph.h"
#include "Log.h"
#include "triangles.h"
#include <chrono>

using namespace std;
using namespace tip;


struct Vertex {
    Vertex(int elem) : elem(elem), marked(false) {}

    int elem;
    mutable bool marked;

    void marcar(bool val) const {
        marked = val;
    }

    bool marcado() const {
        return marked;
    }

};

using IntGraph = tip::Graph<Vertex>;

ostream& operator<<(ostream& os, const Vertex& v) {
    return os << v.elem;
}

ostream& operator<<(ostream& os, IntGraph::const_vertex_iterator v) {
    return os << v->elem;
}




int main(int argc, char*argv[]) {
Log::get_instance().set_output(&std::cout);
Log::get_instance().set_level(Log::Level::NONE);

    if(argc < 2) {
        cerr << "Incluir el archivo de entrada" << endl;
        return 1;
    }


    ifstream ficheroEntrada(argv[1]);
    ofstream file_time("gnp.ods"); // archivo que guarda registro de tiempo

    if(not ficheroEntrada.is_open()) {
        cerr << "Fichero inexistente" << endl;
        return 2;
    }

    auto start = chrono::steady_clock::now(); //inicio del reloj

    IntGraph G;
    vector<IntGraph::const_vertex_iterator> V;

    int cant_vertices, cant_aristas,deg,sqrt;

    ficheroEntrada >> cant_vertices >> cant_aristas;

    //agregamos los vertices
    for(int v = 0; v < cant_vertices; v++) {
        V.push_back(G.insert_vertex(v));
    }

    for(int i = 0; i < cant_aristas; ++i)
    {
        int vertex, vecino;
        //crearmos las aristas.
        ficheroEntrada >> vertex >> vecino;
        G.add_edge(V[vertex], V[vecino]);
    }
    ficheroEntrada >> deg >> sqrt;
    ficheroEntrada.close();
    cerr << G << endl;

    cout << "  [h-graph] searching for triangles" << endl;

    ofstream ficheroSalida(argv[1] + string(".triangles"));

    auto T = triangles(G);
    for(auto t = T.begin(); t != T.end(); ++t) {
        ficheroSalida << t->v << " " << t->w << " " << t->z << endl;
    }

    auto end = chrono::steady_clock::now();//fin del reloj
    auto diff = end - start;
    auto density = float(cant_aristas *2) / (G.vertexCount()*(G.vertexCount()-1));

    file_time << G.vertexCount()<<" ;" <<cant_aristas<<" ;" << (float)chrono::duration <double, milli> (diff).count()<<";" << "ms "<<"; "<<density<<";"<<"density"<<";"<<deg
    <<";"<<"degeneraty"<<";"<<sqrt<<";"<<"m*sqrt(m)"<<";"<<deg/2<<endl;

    cout << "  [h-graph] storing the graph" << endl;
    ofstream grafoSalida(string(argv[1]) + string(".graph"));
    grafoSalida << cant_vertices << " " << cant_aristas << endl;
    for(auto v = V.begin(); v != V.end(); ++v) {
        for(auto w = v->begin(); w != v->end(); ++w) {
            if((*v)->elem < w->elem) {
                grafoSalida << *v << " " << w << endl;
            }
        }
     grafoSalida<<deg<<" "<<sqrt<<endl;
    }

    ficheroSalida.close();

    file_time.close();





    return 0;

}
