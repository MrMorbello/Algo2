#!/usr/bin/python3

import sys
from typing import List
from red import Red


def camino(red: Red, args: List):
    lista = ' '.join(args).split(' >>>> ')
    origen, destino = lista[0], lista[1]
    red.camino(origen, destino)

def mas_importantes(red: Red, args: List):
    cantidad = int(args[0])
    red.mas_importantes(cantidad)

def recomendacion(red: Red, args: List):
    modo = args[0]
    cantidad = int(args[1])
    lista = ' '.join(args[2:]).split(' >>>> ')
    red.recomendacion(modo, cantidad, lista)

def ciclo(red: Red, args: List):
    cantidad = int(args[0])
    cancion = ' '.join(args[1:])
    red.ciclo(cantidad, cancion)

def rango(red: Red, args: List):
    cantidad = int(args[0])
    cancion = ' '.join(args[1:])
    red.rango(cantidad, cancion)

def comando_desconocido(red: Red, args: List):
    print("Error: Comando desconocido")

def main():
    '''
    Inicializo la red y leo los comandos, para después llamar a la función
    correspondiente (con los argumentos que serán separados en dicha función)
    '''
    
    if len(sys.argv) != 2: return
    ruta_archivo = sys.argv[1]

    red = Red(ruta_archivo)
    
    comandos = {
        "camino" : camino,
        "mas_importantes": mas_importantes,
        "recomendacion": recomendacion,
        "ciclo": ciclo,
        "rango": rango
    }

    for entrada in sys.stdin:
        comando_y_args = entrada.split()
        # Comando es una lista a partir de la entrada, de donde
        # saco el comando y los argumentos

        funcion = comandos.get(comando_y_args[0], comando_desconocido)
        funcion(red, comando_y_args[1:])
        

if __name__ == '__main__':
    main()
