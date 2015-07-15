#include <iostream>
using namespace std;
#include <list>
#include "Vertex.h"
#include "VertexDescriptor.h"
#include "VertexIterator.h"
#include "EdgeIterator.h"


#ifndef GRAPH_H
#define GRAPH_H



class Graph{

public:

 Graph() { }

list<VertexDescriptor> vertexs;

/**
	 * Inserta un nuevo vertice al grafo y retorna el numero del indice agregado.
	 *
	 * En caso de querer utilizar otro tipo de identificadores para los vertices,
	 * se necesita algun tipo de diccionario externo.  En este proyecto no tomamos
	 * este problema en cuenta
	 *
	 * @return numero del vertice agregado
	 */
Vertex addVertex(unsigned int elem);

	/**
	 * Elimina el vertice v del grafo si el mismo pertenece al grafo.
	 * Si no pertenece no hace nada.
	 *
	 * @param v vertice a eliminar
	 */
void removeVertex(unsigned int v);

/**
	 * Agrega la arista vw al grafo.  Si la arista ya existe, no se agrega.
	 *
	 * @param v uno de los vertices de la arista
	 * @param w el otro vertice de la arista
	 */

void addEdge(unsigned int v, unsigned int w);

/**
	 * Elimina la arista vw del grafo.  Si no pertenece al grafo, no hace nada
	 *
	 * @param v uno de los vertices de la arista
	 * @param w el otro vertice de la arista
	 */
void removeEdge(unsigned int v, unsigned int w);


/**
* @return cantidad de vertices del grafo
*/
int vertexCount() ;

/**
*
* @param v
* @return grado de un vertex
*/
int* degree(Vertex *v);

/**
	 * Pertenencia de la arista vw
	 *
	 * @param v
	 * @param w
	 * @return true si la arista pertenece al grafo.
	 */
bool hasEdge(Vertex v, Vertex w);


/**
 * Retorna un iterador a los vecinos de v
 *
 * @param args
*/
VertexIterator iterNeighbors();

/**
	 * Retorna un iterador a los vecinos de v que tienen grado al menos d(v)
	 *
	 * @param v
	 * @return iterador a los vecinos de v.
*/
VertexIterator iterHighNeighbors(int v);


/**
	 * Retorna un iterador que nos permite recorrer N'(v)
	 *
	 * @param v
	 * @return
	 */
EdgeIterator iterEdgeNeighbors(int v);








};

#endif //CONJACOTADO_H


