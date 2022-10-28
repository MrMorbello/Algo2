#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "pila.h"

typedef struct nodo nodo_t;

struct nodo {
    nodo_t* izq;
    nodo_t* der;
    char* clave;
    void* dato;
};

struct abb {
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t fcomp;
    abb_destruir_dato_t fdestr;
};

// Auxiliares

nodo_t* nodo_crear(char* clave) {
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (!nodo) return NULL;

    nodo->clave = clave;
    nodo->dato = NULL;

    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}

nodo_t** buscar_en_nodos(abb_comparar_clave_t fcomp, nodo_t* nodo, const char* clave, bool* encontrado) {
    // Devuelve puntero al puntero que tiene el padre del nodo.
    
    if (!nodo) {
        return NULL;
    }
    int comp = fcomp(clave, nodo->clave);

    if (comp > 0) {
        if (!nodo->der) return &(nodo->der);
        if (fcomp(clave, nodo->der->clave) == 0) {
            *encontrado = true;
            return &(nodo->der);
        }
        return buscar_en_nodos(fcomp, nodo->der, clave, encontrado);
    }

    if (comp < 0) {
        if (!nodo->izq) return &(nodo->izq);
        if (fcomp(clave, nodo->izq->clave) == 0) {
            *encontrado = true;
            return &(nodo->izq);
        }
        return buscar_en_nodos(fcomp, nodo->izq, clave, encontrado);
    }

    return NULL;
}

nodo_t** buscar_puntero(abb_t* abb, const char* clave, bool* encontrado) {
    if (!abb->raiz) {
        return &(abb->raiz);
    }
    if (abb->fcomp(abb->raiz->clave, clave) == 0) {
        *encontrado = true;
        return &(abb->raiz);
    }
    return buscar_en_nodos(abb->fcomp, abb->raiz, clave, encontrado);
}

nodo_t** buscar_siguiente(nodo_t* nodo) {
    nodo_t* act = nodo->der;

    if(!act->izq) return &(nodo->der);

    while (act->izq->izq) {
        act = act->izq;
    }
    return &(act->izq);
}

void iter_buscar_siguientes(pila_t* pila, nodo_t* nodo) {
    nodo_t* act = nodo;

    while (act) {
        pila_apilar(pila, (void*)act);
        act = act->izq;
    }
}

void destruir_post_orden(nodo_t* nodo, abb_destruir_dato_t fdestr) {
    if (!nodo) return;

    destruir_post_orden(nodo->izq, fdestr);
    destruir_post_orden(nodo->der, fdestr);

    if (fdestr) fdestr(nodo->dato);
    free(nodo->clave);
    free(nodo);
}


// Primitivas ABB

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    if (!cmp) return NULL;

    abb_t* abb = malloc(sizeof(abb_t));

    if (!abb) return NULL;

    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->fcomp = cmp;
    abb->fdestr = destruir_dato;

    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {

    bool encontrado = false;
    nodo_t** puntero_nodo = buscar_puntero(arbol, clave, &encontrado);
    nodo_t* nodo;

    if (encontrado) {
        nodo = *puntero_nodo;
        if (arbol->fdestr) arbol->fdestr(nodo->dato);
    } else {
        char* clave_copia = strdup(clave);

        if (!clave_copia) return false;

        nodo = nodo_crear(clave_copia);

        if (!nodo) {
            free(clave_copia);
            return false;
        }

        *puntero_nodo = nodo;
        arbol->cantidad += 1;
    }

    nodo->dato = dato;
    
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    bool encontrado = false;
    nodo_t** puntero_nodo = buscar_puntero(arbol, clave, &encontrado);

    if (!encontrado) return NULL;

    nodo_t* nodo_b = *puntero_nodo;
    void* dato_b = nodo_b->dato;
    char* clave_b = nodo_b->clave;
    
    if (nodo_b->izq && nodo_b->der) {
        nodo_t** reemplazo = buscar_siguiente(nodo_b);

        nodo_t* nodo_reemplazo = *reemplazo;
        // El nodo sólo tendrá derecha.
        *reemplazo = (*reemplazo)->der;

        nodo_b->clave = nodo_reemplazo->clave;
        nodo_b->dato = nodo_reemplazo->dato;

        nodo_b = nodo_reemplazo;
    } 

    else if (nodo_b->der) *puntero_nodo = nodo_b->der;
    
    else if (nodo_b->izq) *puntero_nodo = nodo_b->izq;

    else *puntero_nodo = NULL;


    free(nodo_b);
    
    free(clave_b);
    arbol->cantidad -= 1;

    return dato_b;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    if (arbol->raiz && arbol->fcomp(arbol->raiz->clave, clave) == 0) return arbol->raiz->dato;

    bool encontrado = false;
    nodo_t** puntero_nodo = buscar_en_nodos(arbol->fcomp, arbol->raiz, clave, &encontrado);

    if (!encontrado) return NULL;

    return (*puntero_nodo)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    if (arbol->raiz && arbol->fcomp(arbol->raiz->clave, clave) == 0) return true;
    bool encontrado = false;
    buscar_en_nodos(arbol->fcomp, arbol->raiz, clave, &encontrado);
    return encontrado;
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
    if (arbol->cantidad != 0) destruir_post_orden(arbol->raiz, arbol->fdestr);

    free(arbol);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    if (arbol->cantidad == 0) return;

    pila_t* pila = pila_crear();
    if (!pila) return;

    iter_buscar_siguientes(pila, arbol->raiz);
    bool seguir = true;

    while (!pila_esta_vacia(pila) && seguir) {
        nodo_t* nodo = (nodo_t*)pila_desapilar(pila);
        if (nodo->der) iter_buscar_siguientes(pila, nodo->der);

        seguir = visitar(nodo->clave, nodo->dato, extra);
    }

    pila_destruir(pila);
}


// Primitivas Iter ABB
struct abb_iter {
    abb_t* abb;
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));

    if (!iter) return NULL;

    pila_t* pila = pila_crear();

    if (!pila) {
        free(iter);
        return NULL;
    }
    iter->pila = pila;

    iter_buscar_siguientes(iter->pila, arbol->raiz);

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if(abb_iter_in_al_final(iter)) return false;

    nodo_t* act = (nodo_t*)pila_desapilar(iter->pila);

    if(act->der) {
        iter_buscar_siguientes(iter->pila, act->der);
    }

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if(pila_esta_vacia(iter->pila)) return NULL;

    nodo_t* nodo_actual = (nodo_t*)pila_ver_tope(iter->pila);

    return nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}

