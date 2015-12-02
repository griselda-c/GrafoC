import networkx as nx
from __future__ import print_function
G = nx.complete_graph(5)
petersen=nx.petersen_graph()
bull = nx.bull_graph()
diamond = nx.diamond_graph()



def imprimir_grafo(grafo):
     size = len(grafo.nodes())
     print(size)
     for i in grafo.nodes():
             print(i, end=" ")
     for x in grafo.edges():
             L = list(x)
             for z in L:
                     print(z, end=" ")
             print("\n")
     print(size + 1)



def grabar_grafo(grafo):
     archi= open('datos.txt','a')
     size = len(grafo.nodes())
     archi.write(str(size))
     archi.write('\n')
     for i in grafo.nodes():
             archi.write(str(i))
             archi.write('  ')
     for x in grafo.edges():
             L = list(x)
             for z in L:
                     archi.write(str(z))
                     archi.write('  ')
             archi.write('\n')
     archi.write(str(size + 1))
     archi.close()





imprimir_grafo(G)
 


