#include "cola.h"
#include <stdlib.h>


typedef struct nodo {
    void* dato;
    struct nodo* sig;
} nodo_t;

struct cola {
    nodo_t* prim;
    nodo_t* ult;
};

// Cola desencolar <<< | prim ->  Nodo -> Nodo -> ult -> _ | <<< encolar

nodo_t* nuevo_nodo(void* dato) {

    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (nodo == NULL) {
        return NULL;}

    nodo->dato = dato;
    nodo->sig = NULL;

    return nodo;
}

cola_t* cola_crear(void) {

    cola_t* cola = malloc(sizeof(cola_t));

    if (cola == NULL) {
        return NULL;}

    cola->prim = NULL;
    cola->ult = NULL;

    return cola;
}

void cola_destruir(cola_t* cola, void (*destruir_dato)(void *)) {

    nodo_t* nodo = cola->prim;

    while (nodo) {
        
        if (destruir_dato) destruir_dato(nodo->dato);
        
        nodo_t* nodo_aux = nodo; 
        nodo = nodo->sig;

        free(nodo_aux);
    }

    free(cola);
}

bool cola_esta_vacia(const cola_t* cola) {

    return cola->prim == NULL;
}

bool cola_encolar(cola_t* cola, void* valor) {

    nodo_t* nodo = nuevo_nodo(valor);
    if (nodo == NULL) return false;

    if (!cola->prim) {
        cola->prim = nodo;
        cola->ult = cola->prim;
        return true;
    }

    cola->ult->sig = nodo;
    cola->ult = nodo;

    return true;
}

void* cola_ver_primero(const cola_t* cola) {

    if (!cola->prim) return NULL;

    return cola->prim->dato;
}

void* cola_desencolar(cola_t* cola) {

    if (!cola->prim) return NULL;

    void* dato = cola->prim->dato;
    nodo_t* nodo_aux = cola->prim;

    cola->prim = cola->prim->sig;
    free(nodo_aux);

    if (!cola->prim) cola->ult = cola->prim;

    return dato;
}
