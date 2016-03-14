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


    ofstream file_time("dense.ods"); // archivo que guarda registro de tiempo
    ofstream log_file("dense.log");
    auto fin = 99;

    for(int i = 0; i <= fin; ++i){

        ifstream file_entry;
        auto file = "out." +std::to_string(i);

        file_entry.open (file, ios::in);

        if(not file_entry.is_open()) {
            cerr << "Fichero inexistente" << endl;
            return 2;
        }

       auto start = chrono::steady_clock::now(); //inicio del reloj
        IntGraph G;
        vector<IntGraph::const_vertex_iterator> V;

        int numb_nodes, numb_edges,degeneracy,operation;
        file_entry >> numb_nodes >> numb_edges;

        //agregamos los vertices
        for(int v = 0; v < numb_nodes; v++) {
            V.push_back(G.insertVertex(v));
        }

        for(int i = 0; i < numb_edges; ++i)
        {
            int vertex, vecino;
            //crearmos las aristas.
            file_entry >> vertex >> vecino;
            G.add_edge(V[vertex], V[vecino]);
        }

        file_entry >>degeneracy>>operation;

        file_entry.close();

        //busca triangulos
        auto T = triangles(G);
        for(auto t = T.begin(); t != T.end(); ++t) {
            cout<< t->v << " " << t->w << " " << t->z << endl;
        }
        auto end = chrono::steady_clock::now();//fin del reloj
        auto diff = end - start;

        auto density = float(numb_edges *2) / (G.vertexCount()*(G.vertexCount()-1));



        file_time << G.vertexCount()<<" ;" <<numb_edges<<" ;" << chrono::duration <double, milli> (diff).count()<<";" << "ms "<<"; "<<density<<";"<<"density"<<";"<<degeneracy
        <<";"<<"degeneraty"<<";"<<operation<<";"<<"operation"<<endl;
        log_file<< chrono::duration <double, milli> (diff).count()<<",";

    }

    file_time.close();
    log_file.close();
    return 0;
}
