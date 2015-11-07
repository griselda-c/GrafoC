import networkx as nx
from __future__ import print_function
G = nx.complete_graph(5)


def imprimir_grafo(grafo):
     print( len(grafo.nodes()), end="")
     for i in G.nodes():
             print(i, end="")
     for x in G.edges():
             L = list(x)
             for z in L:
                     print(z, end="")


imprimir_grafo(G)
 


