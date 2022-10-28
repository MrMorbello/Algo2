#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"

#define CAPACIDAD_INICIAL 100
#define FACTOR_REDIMENSION 2

struct heap {
    cmp_func_t cmp;
    size_t capacidad;
    size_t cantidad;
    void** arr;
};

// Funciones Auxiliares

bool heap_redimensionar(heap_t* heap) {
    void** arr_aux = realloc(heap->arr, FACTOR_REDIMENSION * heap->capacidad * sizeof(void*));
        
    if(!arr_aux) return false;

    heap->arr = arr_aux;
    heap->capacidad *= FACTOR_REDIMENSION;

    return true;
}

void swap(void** arr, size_t x, size_t y) {
    // Intercambia en dos posiciones de un arreglo
    void* aux = arr[x];
    arr[x] = arr[y];
    arr[y] = aux;
}

void upheap(void** arr, cmp_func_t cmp, size_t largo, size_t pos) {
    if (pos == 0 || pos >= largo) return;

    size_t  pos_padre = (pos - 1) / 2;

    if(cmp(arr[pos], arr[pos_padre]) > 0) {
        swap(arr, pos, pos_padre);
        upheap(arr, cmp, largo, pos_padre);
    }
}

void downheap(void** arr, cmp_func_t cmp, size_t largo, size_t pos_padre) {
    size_t h_izq = (pos_padre * 2) + 1;
    if(h_izq >= largo) return; // Si no hay izq, no hay der.

    size_t h_mayor = h_izq;
    size_t h_der = (pos_padre * 2) + 2;

    if(h_der < largo) {
        h_mayor = cmp(arr[h_izq], arr[h_der]) > 0 ? h_izq : h_der;
    }

    if(cmp(arr[h_mayor], arr[pos_padre]) > 0) {
        swap(arr, h_mayor, pos_padre);
        downheap(arr, cmp, largo, h_mayor);
    }
    
}

void heapify(void** arr, cmp_func_t cmp, size_t largo) {
    for(int i = (int)largo / 2; i >= 0; i--) {
        downheap(arr, cmp, largo, (size_t)i);
    }
}

// Primitivas del Heap

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cmp, cant);

    for(size_t i = 0; i < cant; i++) {
        swap(elementos, 0, cant - i - 1);
        downheap(elementos, cmp, cant - i - 1, 0);
    }
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));

    if(!heap) return NULL;

    heap->arr = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if(!heap->arr) return NULL;

    heap->cmp = cmp;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cantidad = 0;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));

    if(!heap) return NULL;

    heap->arr = malloc(FACTOR_REDIMENSION * n * sizeof(void*));

    if(!heap->arr) return NULL;

    for(size_t i = 0; i < n; i++) {
        heap->arr[i] = arreglo[i];
    }

    heapify(heap->arr, cmp, n);

    heap->cantidad = n;
    heap->capacidad = FACTOR_REDIMENSION * n;
    heap->cmp = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cantidad; i++) {
            destruir_elemento(heap->arr[i]);
        }
    }

    free(heap->arr);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if(heap->cantidad == heap->capacidad - 1) {
        if(!heap_redimensionar(heap)) return false;
    }

    heap->arr[heap->cantidad] = elem;
    heap->cantidad += 1;

    upheap(heap->arr, heap->cmp, heap->cantidad, heap->cantidad - 1);

    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if(heap_esta_vacio(heap)) return NULL;
    return heap->arr[0];
}

void *heap_desencolar(heap_t *heap) {
    if(heap_esta_vacio(heap)) return NULL;

    void* dato = heap->arr[0];
    heap->cantidad -= 1;

    if(heap->cantidad == 0) return dato;
    swap(heap->arr, heap->cantidad, 0);
    downheap(heap->arr, heap->cmp, heap->cantidad, 0);

    return dato;
}
