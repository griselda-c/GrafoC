import networkx as nx
import sys
from subprocess import call

def read_data(string):
    lines = [line for line in string.split('\n') if line != '']
    return [line.split(' ') for line in lines]

def as_graph(string):
    as_list = [(int(v), int(w)) for v,w in read_data(string)]
    G = nx.graph.Graph()
    G.add_nodes_from(xrange(0, as_list[0][0]))
    G.add_edges_from(as_list[1:])
    return G

def read_test_triangles(string):
    as_list = [sorted([int(u), int(v), int(w)]) for u,v,w in read_data(string)]
    as_list.sort(lambda (u,v,w), (a,b,c) : cmp(u, a) if cmp(u,a) != 0 else (cmp(v,b) if cmp(v,b) != 0 else cmp(w,c)))
    return as_list
                                                      

def triangles(G):
    #suponemos que los vertices van de 0 a n-1
    res = []
    for u in xrange(0, G.order()):
        for v in xrange(u+1, G.order()):
            if not v in G[u]: 
                continue
            for w in xrange(v+1, G.order()):
                if w in G[u] and w in G[v]:
                    res += [[u, v, w]]
    return res

if __name__ == "__main__":
        
    if len(sys.argv) < 2:
        print "pasar el nombre del archivo con el grafo a probar"
        sys.exit(1)
    
    n = 0
    while(True):
        try:
            print "testing", n
            print "  opening graph file"
            graph = as_graph(open(sys.argv[1] + '.' + str(n)).read())
            print "  calling h-graph program"
            call(["./triangles", sys.argv[1] + '.' + str(n)])
            print "  testing the integrity of the h-graph",
            hgraph = as_graph(open(sys.argv[1] + '.' + str(n) + '.graph').read())
            if sorted(hgraph.edges()) != sorted(graph.edges()):
                print "FAILED"
            else:
                print "OK"
                
            print "  reading the output from h-graph"
            test = read_test_triangles(open(sys.argv[1] + '.' + str(n) + '.triangles').read())
            
            print "  brute force finding the triangles"
            the_triangles = triangles(graph)
            if test != the_triangles:
                print "  FAILED!!!!:",
                print str([t for t in test if not t in the_triangles]), "appear in test but are not triangles, while",
                print str([t for t in the_triangles if not t in test]), "are triangles but do not appear in test."                
            else: 
                print "  TEST OK!!!"
                
            n += 1
        except:
            break