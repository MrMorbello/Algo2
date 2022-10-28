from grafo import Grafo
from biblioteca import *
    
def aristas(grafo:Grafo):
    grafo.añadir_arista(1,4)
    grafo.añadir_arista(2,4)
    grafo.añadir_arista(3,4)
    grafo.añadir_arista(4,5)
    grafo.añadir_arista(7,5)
    grafo.añadir_arista(8,5)
    grafo.añadir_arista(7,8)
    grafo.añadir_arista(5,6)
    grafo.añadir_arista(9,6)
    grafo.añadir_arista(10,6)
    grafo.añadir_arista(10,9)
    grafo.añadir_arista(6,4)

def main():
    grafo = Grafo()
    for i in range(1, 11):
        grafo.añadir_vertice(i)
    aristas(grafo)

    pr = crear_pagerank(grafo)
    rank = mejores_segun_rango(pr)
    print(rank)
main()