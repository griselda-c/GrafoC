import networkx as nx
from __future__ import print_function
#G = nx.complete_graph(5)
#petersen=nx.petersen_graph()


def imprimir_grafo(grafo):
     print( len(grafo.nodes()), end="")
     for i in grafo.nodes():
             print(i, end="")
     for x in grafo.edges():
             L = list(x)
             for z in L:
                     print(z, end="")


imprimir_grafo(G)
 


