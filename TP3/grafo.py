class Grafo:
    def __init__(self, dirigido = False):
        self.es_dirigido = dirigido
        self.vertices = {}

    def pertenece(self, vertice):
        return vertice in self.vertices

    def añadir_vertice(self, vertice):
        self.vertices[vertice] = self.vertices.get(vertice, {})
    
    def hay_arista(self, v, w):
        '''
        Devuelve la tupla (existe, peso). Si no existe, el peso será 0
        '''
        if v not in self.vertices or w not in self.vertices: return (False, 0)
        return (w in self.vertices[v], self.vertices[v].get(w, 0))

    def añadir_arista(self, v, w, peso=1):
        '''
        Establece una arista entre v y w, con el peso dado, y arista inversa si no es dirigido
        '''
        self.vertices[v][w] = peso
        if not self.es_dirigido:
            self.vertices[w][v] = peso
    
    def adyacentes(self, vertice):
        '''
        Devuelve una lista de vertices adyacentes al vertice dado
        '''
        lista = []
        for ady in self.vertices.get(vertice, {}):
            lista.append(ady)
        
        return lista

    def cantidad_de_adyacentes(self, vertice):
        return len(self.vertices[vertice])

    def obtener_vertices(self):
        '''
        Devuelve una lista con todos los vertices del grafo
        '''
        lista = []
        for v in self.vertices:
            lista.append(v)
        return lista
    
    def __len__(self):
        return len(self.vertices)
