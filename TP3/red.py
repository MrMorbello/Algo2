from grafo import Grafo
from biblioteca import *


class Red:
    def __init__(self, nombre_archivo):
        '''
        Pre: Siempre llega un nombre_archivo válido
        '''
        self.grafo_bipartito = Grafo(dirigido=False)
        self.grafo_canciones = Grafo(dirigido=False)
        cargar_tsv(nombre_archivo, self.grafo_bipartito, self.grafo_canciones)
        self.canciones_vinculadas = False
        self.ranking = []
        self.ranking_calculado = False

    def camino(self, origen, destino):
        '''
        Imprime una lista con la cual se conecta (en la menor cantidad de pasos posibles) una canción con otra, 
        considerando los usuarios intermedios y las listas de reproducción en las que aparecen.

        Ej: origen --> aparece en playlist --> N_Playlist --> de --> N_User --> tiene una playlist --> N_Playlist* 
        --> donde aparece --> N_Canción --> aparece en playlist --> ... --> donde aparece --> destino

        Complejidad: Este comando debe ejecutar en O(U+C+L), siendo U la cantidad de Usuarios, C la 
        cantidad de canciones, y L la cantidad de apariciones totales de canciones en listas de 
        reproducción (las aristas del grafo).

        Pre: Debo validar en esta función el origen y destino como canciones con autor válidas 
        Post: en caso contrario, imprimir "Tanto el origen como el destino deben ser canciones"
        '''
        if not (self.grafo_canciones.pertenece(origen) and self.grafo_canciones.pertenece(destino)):
            print("Tanto el origen como el destino deben ser canciones")
            return

        padre, _ = bfs(self.grafo_bipartito, origen)

        if not destino in padre:
            print("No se encontro recorrido")
            return

        lista = reconstruir_camino(padre, origen, destino)
        texto = ''

        for i in range(len(lista)):
            actual = lista[i]

            if i % 2 == 0: # Cancion
                texto += f"{actual}"
            else: # Usuario
                # Si estoy en un usuario, hay cancion anterior y cancion siguiente
                _, playlist_anterior = self.grafo_bipartito.hay_arista(lista[i - 1], actual)
                _, playlist_siguiente = self.grafo_bipartito.hay_arista(lista[i + 1], actual)
                texto += f' --> aparece en playlist --> {playlist_anterior} --> de --> {actual} --> tiene una playlist --> {playlist_siguiente} --> donde aparece --> '

        print(texto)

    def mas_importantes(self, cantidad):
        '''
        Muestra las n canciones más centrales/importantes del mundo según el algoritmo de 
        pagerank, ordenadas de mayor a menor importancia, separadas por ;.
        Pre: cantidad es un entero
        '''

        if not self.ranking_calculado:

            pagerank = crear_pagerank(self.grafo_bipartito)

            rank = mejores_segun_rango(pagerank)
            self.ranking = list(filter(lambda v: self.grafo_canciones.pertenece(v), rank))

            self.ranking_calculado = True
        
        print('; '.join(self.ranking[:cantidad]))

    def recomendacion(self, modo, cantidad, lista_de_canciones):
        '''
        Dar una lista de n usuarios o canciones para recomendar, dado el 
        listado de canciones que ya sabemos que le gustan a la persona a la cual recomedar.

        Pre: modo es una cadena "usuarios" o "canciones" con la que elegimos la operación.
        Cantidad es un entero
        '''
        originales = set(lista_de_canciones)
        rank = pagerank_personalizado(self.grafo_bipartito, originales)

        if modo == 'canciones':
            lista = list(filter(lambda x: self.grafo_canciones.pertenece(x) and x not in originales, rank))

        if modo == 'usuarios':
            lista = list(filter(lambda x: not self.grafo_canciones.pertenece(x), rank))
        
        print('; '.join(lista[:cantidad]))
    
    def ciclo(self, largo, cancion):
        '''
        Devuelve un ciclo de largo dado dentro de la red de canciones que 
        comienza con la canción indicada (sale y vuelve a la misma canción)

        Complejidad: Este comando debe ejecutar en O(C^n), pero realizando 
        una buena poda puede reducirse sustancialmente el tiempo de ejecución.

        Pre: largo es un entero, canción una cadena
        Post: En caso de no haber un ciclo de dicho largo empezando desde la 
        cancion dada, debe escribirse por salida estándar "No se encontro recorrido".
        '''
        if not self.canciones_vinculadas: 
            vincular_canciones(self.grafo_bipartito, self.grafo_canciones)
            self.canciones_vinculadas = True
        
        encontrado, lista = ciclo_dfs(self.grafo_canciones, largo, cancion, set(), cancion)

        if encontrado: print(' --> '.join(lista))
        else: print('No se encontro recorrido')

    def rango(self, saltos, cancion):
        '''
        Devuelve la cantidad de canciones que se encuenten a 
        exactamente n saltos desde la cancion pasada por parámetro.

        Complejidad: Este comando debe ejecutar en O(C+L).

        Pre: largo es un entero, canción una cadena
        '''
        if not self.canciones_vinculadas: 
            vincular_canciones(self.grafo_bipartito, self.grafo_canciones)
            self.canciones_vinculadas = True
        
        _, orden = bfs(self.grafo_canciones, cancion)
        canciones_en_rango = contar_claves_con_valor(orden, saltos)
        print(canciones_en_rango)
