import networkx as nx
from networkx.generators.random_graphs import connected_watts_strogatz_graph as real_world
from networkx.generators.random_graphs import gnp_random_graph
from networkx.generators.random_graphs import dense_gnm_random_graph
import math
import sys


def as_string(grafo):
    degeneracy = max(nx.core_number(grafo).values())*grafo.size()
    operation = grafo.size()/math.sqrt(grafo.size())
    return '\n'.join(str(v) + ' ' + str(w) for v, w in [(grafo.order(), grafo.size())] + grafo.edges()) +'\n'+str(degeneracy)+'\n'+str(operation)    
     


def grabar_grafo(grafo, filename="datos.txt"):     
     archi= open(filename,'w')
     archi.write(as_string(grafo))
     archi.close()

def generar_grafo(ngraphs,filename):
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
        nodos = 10+5*i
	#newman = nx.newman_watts_strogatz_graph(nodos,4,0.3)
        #erdos = nx.erdos_renyi_graph(nodos,0.15)
	#real = real_world(nodos, 4, 0.3)
        #gnp = gnp_random_graph(nodos,0.10)
	dense = dense_gnm_random_graph(10+5*i, (100+5*i)*2)
        grabar_grafo(dense, filename + '.' + str(i))



if __name__ == '__main__':

    

    ngraphs = 1 if len(sys.argv) == 1 else int(sys.argv[1])
    filename = "out" if len(sys.argv) <= 2 else sys.argv[2]
    
    print "cantidad de grafos a generar:", ngraphs
    
    generar_grafo(ngraphs,filename)

    
   

   
