class _Nodo:
    def __init__(self, dato, prox = None):
        self.dato = dato
        self.prox = prox

class Cola:
    def __init__(self):
        self.frente = None
        self.ultimo = None
    
    def encolar(self, dato):
        nuevo = _Nodo(dato)
        if self.ultimo is None:
            self.ultimo = nuevo
            self.frente = nuevo
        else:
            self.ultimo.prox = nuevo
            self.ultimo = nuevo
        
    def desencolar(self):
        dato = self.frente.dato
        self.frente = self.frente.prox
        if self.frente is None:
            self.ultimo = None
        return dato
    
    def ver_tope(self):
        return self.frente.dato
    
    def esta_vacia(self):
        return self.frente is None