# estructura h-grafo

En el archivo generador.py se elige el tipo de grafo a crear. Se pueden elegir entre las opciones:

0 = newman,
1 = erdos,
2 = real,
3 = gnp,
4 = dense,
5 = camino

Entonces en el main:

          generar_grafo(ngraphs,filename,opcion)

opcion es un número entre 0 y 5.

Para correr los test. Ejecutar el script de python generador.py junto a la cantidad de grafos a crear.
  
          python generador.py 100
  
   
Luego ejecutar el script triangles.py

        python triangles.py out
  
out es el nombre del archivo que genera el generador.


  
