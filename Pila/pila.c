#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_BASE 2

/* Definición del struct pila proporcionado por la cátedra.*/

struct pila {
    void** datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/


pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }

    void** datos = malloc(2 * sizeof(void*));

    if (datos == NULL) {
        free(pila);
        return NULL;
    }
    
    pila->datos = datos;
    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD_BASE;

    return pila;
}

bool pila_redimensionar(pila_t* pila, size_t capacidad_nueva) {

    void** datos_nuevo = realloc(pila->datos, capacidad_nueva * (sizeof(void*)));
    if (datos_nuevo == NULL) return false;

    pila->datos = datos_nuevo;
    pila->capacidad = capacidad_nueva;
    
    return true;
}

void pila_destruir(pila_t* pila) {
    if (!pila) return;
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t* pila) {
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t* pila, void* valor) {
    if (!pila) return false;
    bool termino_bien = true;

    pila->cantidad += 1;

    size_t tope = pila->cantidad - 1;

    pila->datos[tope] = valor;

    if (pila->cantidad == pila->capacidad){
        termino_bien = pila_redimensionar(pila, 2 * pila->capacidad);
    }

    return termino_bien;
}

void* pila_ver_tope(const pila_t* pila) {
    if (!pila || pila->cantidad <= 0) return NULL;

    return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t* pila) {
    if (!pila || pila->cantidad <= 0) return NULL;

    pila->cantidad -= 1;

    void* dato_desapilado = pila->datos[pila->cantidad];

    if (pila->capacidad > CAPACIDAD_BASE && 4 * pila->cantidad <= pila->capacidad){ //Compruebo que haya un mínimo de capacidad (explícito)
        pila_redimensionar(pila, pila->capacidad / 2);
    }

    return dato_desapilado;
}

