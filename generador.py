import networkx as nx
from networkx.generators.random_graphs import connected_watts_strogatz_graph as real_world
from networkx.generators.random_graphs import gnp_random_graph
from networkx.generators.random_graphs import dense_gnm_random_graph
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

    '''
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
	nodos = 10+5*i
        grabar_grafo(nx.newman_watts_strogatz_graph(nodos,4,0.3), filename + '.' + str(i))
    '''
    '''
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
	nodos = 10+5*i
        grabar_grafo(nx.erdos_renyi_graph(nodos,0.035), filename + '.' + str(i))
    '''
    
    
 
    '''
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
        grabar_grafo(real_world(10+5*i, 4, 0.3), filename + '.' + str(i))
    '''
    '''
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
        grabar_grafo(gnp_random_graph(10+5*i,0.03), filename + '.' + str(i))
    '''
    
    for i in xrange(0,ngraphs):
        print "   generando grafo", i
	nodos = 100+5*i
        grabar_grafo(dense_gnm_random_graph(10+5*i, nodos*2), filename + '.' + str(i))

   
