#include <stdlib.h>
#include "lista.h"


typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

nodo_t* nodo_crear(void* dato) {

    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->prox = NULL;

    return nodo;
}

struct lista {
    size_t largo;
    nodo_t* prim;
    nodo_t* ult;
};


struct lista_iter {
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
};

// Lista enlazada
lista_t* lista_crear(void) {

    lista_t* lista = malloc(sizeof(lista_t));

    if (!lista) return NULL;

    lista->largo = 0;
    lista->prim = NULL;
    lista->ult = NULL;

    return lista;
}

bool lista_esta_vacia(const lista_t* lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t* lista, void* dato) {

    nodo_t* nodo = nodo_crear(dato);
    if (!nodo) return false;

    if (!lista->prim) {
        lista->prim = nodo;
        lista->ult = lista->prim;
        lista->largo = 1;
        return true;
    }

    nodo->prox = lista->prim;
    lista->prim = nodo;
    lista->largo += 1;
    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void *dato) {

    nodo_t* nodo = nodo_crear(dato);
    if (!nodo) return false;

    if (!lista->prim) {
        lista->prim = nodo;
        lista->ult = lista->prim;
        lista->largo = 1;
        return true;
    }

    lista->ult->prox = nodo;
    lista->ult = nodo;
    lista->largo += 1;
    return true;
}

void* lista_borrar_primero(lista_t* lista) {

    if (!lista->prim) return NULL;

    nodo_t* nodo_aux = lista->prim;

    void* dato = nodo_aux->dato;
    lista->prim = nodo_aux->prox;
    lista->largo -= 1;
    if (!lista->prim) lista->ult = NULL;

    free(nodo_aux);

    return dato;
}

void* lista_ver_primero(const lista_t* lista) {
    if (!lista->prim) return NULL;
    return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista) {
    if (!lista->ult) return NULL;
    return lista->ult->dato;
}

size_t lista_largo(const lista_t* lista) {
    return lista->largo;
}

void lista_destruir(lista_t* lista, void (*destruir_dato)(void *)) {

    nodo_t* nodo = lista->prim;

    while (nodo) {
        
        if (destruir_dato) destruir_dato(nodo->dato);
        
        nodo_t* nodo_aux = nodo; 
        nodo = nodo->prox;

        free(nodo_aux);
    }

    free(lista);
}

// Iterador Interno
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {

    nodo_t* actual = lista->prim;

    while (actual) {
        if (!visitar(actual->dato, extra)) return;
        actual = actual->prox;
    }
}

//Iterador Externo
lista_iter_t* lista_iter_crear(lista_t* lista) {
    
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));

    if (!iter) return NULL;
    
    iter->lista = lista;
    iter->actual = lista->prim;
    iter->anterior = NULL;

    return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
    if (!iter->actual) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;

    return true;
}

void* lista_iter_ver_actual(const lista_iter_t *iter) {
    if (!iter->actual) return NULL;

    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t* nuevo_nodo = nodo_crear(dato);

    if (!nuevo_nodo) return false;
    nuevo_nodo->prox = iter->actual;

    if (!iter->anterior) iter->lista->prim = nuevo_nodo;

    else iter->anterior->prox = nuevo_nodo;

    if (!iter->actual) iter->lista->ult = nuevo_nodo;

    iter->actual = nuevo_nodo;

    iter->lista->largo += 1;
    return true;
}

void* lista_iter_borrar(lista_iter_t *iter) {
    if (!iter->actual) return NULL;

    if (!iter->anterior) iter->lista->prim = iter->actual->prox;

    else iter->anterior->prox = iter->actual->prox;

    nodo_t* aux = iter->actual;

    iter->actual = iter->actual->prox;

    iter->lista->largo -= 1;

    if (!iter->actual) iter->lista->ult = iter->anterior;

    void* dato = aux->dato;

    free(aux);

    return dato;
}
