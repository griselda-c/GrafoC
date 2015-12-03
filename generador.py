import networkx as nx
from networkx.generators.random_graphs import connected_watts_strogatz_graph as real_world
import sys


def as_string(grafo):
    return '\n'.join(str(v) + ' ' + str(w) for v, w in [(grafo.order(), grafo.size())] + grafo.edges())         
     


def grabar_grafo(grafo, filename="datos.txt"):
     archi= open(filename,'w')
     archi.write(as_string(grafo))
     archi.close()




if __name__ == '__main__':

    #G = nx.complete_graph(5)
    #petersen=nx.petersen_graph()
    #bull = nx.bull_graph()
    #diamond = nx.diamond_graph()

    ngraphs = 1 if len(sys.argv) == 1 else int(sys.argv[1])
    filename = "out" if len(sys.argv) <= 2 else sys.argv[2]
    
    print "cantidad de grafos a generar:", ngraphs
    
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
        grabar_grafo(real_world(10+5*i, 4, 0.3), filename + '.' + str(i))
