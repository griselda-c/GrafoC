import networkx as nx
from __future__ import print_function
G = nx.complete_graph(5)




def imprimir_grafo(grafo):
        print( len(grafo.nodes()), end="")
        for i in grafo:
                print(i, end="")
        for x in grafo.nodes():
                print( len(grafo.neighbors(x)), end="")
		print(x, end="")
               for w in grafo.neighbors(x):
                       print(w, end="")

imprimir_grafo(G)

