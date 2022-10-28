from grafo import Grafo
from cola import Cola
import random


REPETICIONES_PAGERANK = 15
SALTOS_PAGERANK_PERSONALIZADO = 10


def calcular_pagerank(grafo:Grafo, vertice, pagerank, d = 0.85):
    '''
    Calcula sobre el diccionario pagerank el valor de pr del vertice dado
    '''
    adyacentes = grafo.adyacentes(vertice)

    suma_entrantes = 0
    for v in adyacentes:
        suma_entrantes += (pagerank[v] / grafo.cantidad_de_adyacentes(v))

    pagerank[vertice] = ((1 - d) / len(grafo)) + d * suma_entrantes

def crear_pagerank(grafo:Grafo, d = 0.85):
    '''
    Inicializa el pagerank y llama a calcular el pr para todos los V del grafo, 
    '''
    pagerank = {}
    pr_base = (1 - d) / len(grafo)

    for v in grafo.obtener_vertices():
        pagerank[v] = pr_base

    for _ in range(REPETICIONES_PAGERANK):
        for v in grafo.obtener_vertices():
            calcular_pagerank(grafo, v, pagerank)

    return pagerank

def cargar_tsv(ruta, grafo_bipartito: Grafo, grafo_canciones: Grafo):
    '''
    Carga los datos del TSV dado en los dos grafos, el bipartito de canciones y usaurios,
    y el de canciones, donde posteriormente se añadirán las aristas
    '''
    try:
        with open(ruta, 'r', encoding="utf-8") as archivo_tsv:

            next(archivo_tsv)
            # archivo_tsv = csv.reader(archivo_tsv, delimiter='\t') # linea = linea_leida.rstrip().split('\t')

            for linea_leida in archivo_tsv:
                linea = linea_leida.rstrip().split('\t')
                # linea = ['ID', 'USER_ID', 'TRACK_NAME', 'ARTIST', 'PLAYLIST_ID', 'PLAYLIST_NAME', 'GENRES']
                usuario = linea[1]
                playlist = linea[5]
                cancion = f'{linea[2]} - {linea[3]}'

                if not grafo_bipartito.pertenece(usuario): grafo_bipartito.añadir_vertice(usuario)
                if not grafo_bipartito.pertenece(cancion): grafo_bipartito.añadir_vertice(cancion)
                grafo_bipartito.añadir_arista(usuario, cancion, playlist)

                if not grafo_canciones.pertenece(cancion): grafo_canciones.añadir_vertice(cancion)

    except IOError:
        print(f'Error al leer el archivo {ruta}')

def bfs(grafo: Grafo, v):
    '''
    Realiza un recorrido BFS a partir de V, y devuelve los diccionarios padre y orden.
    Al hacer un función genérica, puedo reutilizarla en varias aplicaciones.
    '''

    visitados = {v}
    padre = {v:None}
    orden = {v:0}
    cola = Cola()
    cola.encolar(v)

    while not cola.esta_vacia():
        v = cola.desencolar()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padre[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                cola.encolar(w)
    return padre, orden

def mejores_segun_rango(pagerank):
    '''
    Recibe el diccionario de pagerank y lo ordena para devolver una lista ordenada.
    '''
    elementos = list(pagerank.items())
    elementos.sort(key=lambda x: x[1], reverse=True)

    lista = list(map(lambda x: x[0], elementos))

    return lista

def pagerank_personalizado(grafo:Grafo, originales:set):
    '''
    Realiza un random walk a partir de los vertices del set originales, y devuelve
    una lista con todos los vertices encontrados, ordenados de mayor a menor "importancia"
    '''
    peso = {}
    for vertice in originales:
        random_walk(grafo, originales, vertice, peso)

    return mejores_segun_rango(peso)
    
def random_walk(grafo:Grafo, originales, vertice, peso, saltos = SALTOS_PAGERANK_PERSONALIZADO):
    '''
    Realiza un pagerank personalizado, partiendo de un valor de 1 y distribuyendo entre los ady.
    Utiliza una cantidad de finida de saltos, y un set de originales que filtra 
    los que no me interesan (porque no van a ser opciones, ya que son de los que partí)
    '''
    transmitido = peso.get(vertice, 1) / grafo.cantidad_de_adyacentes(vertice)
    if saltos == 0: return

    for v in grafo.adyacentes(vertice):
        if v in originales:continue
        peso[v] = peso.get(v, 0) + transmitido
    
    vertice_random = random.choice(grafo.adyacentes(vertice))
    
    random_walk(grafo, originales, vertice_random, peso, saltos - 1)

def reconstruir_camino(padre:dict, principio, actual) -> list:
    '''
    Recibe un diccionario de vertice:padre, un principio y un actual y devuelve 
    recursivamente una lista que reconstruye el camino [principio ... actual]
    '''
    if actual == principio: return [principio]

    lista = reconstruir_camino(padre, principio, padre[actual])
    lista.append(actual)

    return lista

def vincular_canciones(grafo_bipartito:Grafo, grafo_canciones:Grafo):
    '''
    Vinculo las canciones del grafo de canciones en funcion de si son adyacentes al mismo 
    usuario en el grafo bipartito (canciones - usuarios).
    '''
    for v in grafo_bipartito.obtener_vertices():
        if grafo_canciones.pertenece(v): continue

        canciones_de_usuario = grafo_bipartito.adyacentes(v)
        for cancion_1 in canciones_de_usuario:
            for cancion_2 in canciones_de_usuario:
                if cancion_1 == cancion_2: continue
                grafo_canciones.añadir_arista(cancion_1, cancion_2)

def contar_claves_con_valor(dicc, valor_dado):
    '''
    Recibe un diccionario y un valor, y devuelve la cantidad de elementos
    con el mismo valor que hay en el diccionario
    Lo utilizo para la funcion de rango, pero hay una opción más eficiente
    '''
    cantidad = 0
    for _, valor in dicc.items():
        if valor == valor_dado: cantidad += 1
    return cantidad

def ciclo_dfs(grafo, n, origen, visitados, actual):
    '''
    Recibe un grafo no dirigido, una cantidad de saltos, un origen y un set de visitados
    y devuelve si se encontró un ciclo de n cantidad de saltos, junto con una lista que contiene
    el ciclo (al ser no dirigido, lo devuelvo "al revés" de como lo encontré, pero no importa)
    '''
    if actual == origen and n == 0: return True, [actual]
    if n == 0 or actual in visitados: return False, []
    visitados.add(actual)
    for v in grafo.adyacentes(actual):
        encontrado, lista = ciclo_dfs(grafo, n - 1, origen, visitados, v)
        if encontrado:
            lista.append(actual)
            return True, lista
    return False, []
