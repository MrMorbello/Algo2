#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"


size_t CAPACIDAD_BASE = 101;
size_t FACTOR_REDIMENSION = 2;
double CRITERIO_REDIMENSION = 0.7;

typedef enum {
    V,
    O,
    B
} estado_t;

typedef struct campo {
    estado_t estado;
    char* clave;
    void* valor;
} campo_t;

struct hash {
    campo_t* tabla;
    size_t ocupados;
    size_t borrados;
    size_t capacidad;
    hash_destruir_dato_t destructor;
};

struct hash_iter {
    const hash_t* hash;
    size_t actual;
};



// Funciones Auxiliares

// Inicializa toda la tabla en valores vacíos.
void tabla_llenar_vacios(campo_t* tabla, size_t largo) {
    for (size_t i = 0; i < largo; i++) {
        tabla[i].estado = V;
        tabla[i].clave = NULL;
        tabla[i].valor = NULL;
    }
}


// Función que realiza operaciones turbias para obtener el hash (no incluye el módulo en capacidad)
size_t funcion_de_hash(const char* clave) {
    // En este caso utilizamos como función una implementación de Jenkins Hash, 
    // pero puede cambiarse en esta parte del código
    // Documentación en https://en.wikipedia.org/wiki/Jenkins_hash_function

    size_t largo = strlen(clave);
    size_t i = 0;
    size_t hash = 0;

    while (i != largo) {
        hash += (size_t)clave[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}


// Función wrapper para poder modificar la función de hash facilmente
size_t hash_clave(const hash_t* hash, const char* clave) {
    return funcion_de_hash(clave) % hash->capacidad;
}


// Busca en la tabla una clave dada, o un espacio vacío (de corrido desde 
// el índice que corresponde a la clave).
// Post: devuelve el indice de la clave o un espacio vacío siguiente en la tabla.
size_t hash_busquda_indice(const hash_t* hash, const char* clave) {

    size_t i = hash_clave(hash, clave);

    campo_t campo = hash->tabla[i];

    while(campo.estado != V) {
        if (clave && campo.estado == O && strcmp(campo.clave, clave) == 0) return i;

        i++;
        if (i == hash->capacidad) i = 0;

        campo = hash->tabla[i];
    }

    return i;
}

// Función de redimensión para el hash. Rehashea todas las claves guardadas
bool hash_redimensionar(hash_t* hash) {
    campo_t* nueva = malloc(FACTOR_REDIMENSION * hash->capacidad * sizeof(campo_t));

    if(!nueva) return false;

    size_t capacidad_vieja = hash->capacidad;
    campo_t* tabla_vieja = hash->tabla;

    hash->borrados = 0;
    hash->ocupados = 0;
    hash->tabla = nueva;
    hash->capacidad = hash->capacidad * FACTOR_REDIMENSION;

    tabla_llenar_vacios(hash->tabla, hash->capacidad);

    
    for (size_t i = 0; i < capacidad_vieja; i++) {
        if (tabla_vieja[i].estado != O) continue;

        campo_t elem = tabla_vieja[i];
        if (!hash_guardar(hash, elem.clave, elem.valor)) return false;
        free(elem.clave);
    }

    free(tabla_vieja);

    return true;
}


// Devuelve la siguiente posición "ocupada"
size_t hash_iter_hasta_ocupado(hash_iter_t* iter) {

    if (hash_iter_al_final(iter)) return iter->hash->capacidad;

    size_t i = iter->actual;

    campo_t campo = iter->hash->tabla[i];

    while (campo.estado != O) {
        i++;
        if (i == iter->hash->capacidad) return i;
        campo = iter->hash->tabla[i];
    }
    
    return i;
}


// Primitivas Hash
hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));

    if (!hash) return NULL;

    campo_t* tabla = malloc(CAPACIDAD_BASE * sizeof(campo_t));

    if (!tabla) {
        free(hash);
        return NULL;
    }

    tabla_llenar_vacios(tabla, CAPACIDAD_BASE);

    hash->tabla = tabla;
    hash->ocupados = 0;
    hash->borrados = 0;
    hash->capacidad = CAPACIDAD_BASE;
    hash->destructor = destruir_dato;

    return hash;
}


bool hash_guardar(hash_t* hash, const char* clave, void* dato) {

    double factor_carga = (double)(hash->ocupados + hash->borrados) / (double)hash->capacidad;

    if (factor_carga >= CRITERIO_REDIMENSION) {
        if (!hash_redimensionar(hash)) return false;
    }

    size_t indice = hash_busquda_indice(hash, clave);

    bool pertenece = hash->tabla[indice].estado == O && strcmp(hash->tabla[indice].clave, clave) == 0;

    if (pertenece) {
        if (hash->destructor) hash->destructor(hash->tabla[indice].valor);
        hash->tabla[indice].valor = dato;
        return true;
    }

    char* clave_aux = strdup(clave);
    if (!clave_aux) return false;
    
    hash->tabla[indice].estado = O;
    hash->tabla[indice].clave = clave_aux;
    hash->tabla[indice].valor = dato;

    hash->ocupados += 1;

    return true;
}


void* hash_borrar(hash_t* hash, const char* clave) {

    size_t indice = hash_busquda_indice(hash, clave);

    bool pertenece = hash->tabla[indice].estado == O && strcmp(hash->tabla[indice].clave, clave) == 0;

    if (!pertenece) return NULL;

    campo_t campo = hash->tabla[indice];
    void* dato = campo.valor;

    free(hash->tabla[indice].clave);

    hash->tabla[indice].clave = NULL;
    hash->tabla[indice].valor = NULL;
    hash->tabla[indice].estado = B;

    hash->borrados += 1;
    hash->ocupados -= 1;

    return dato;
}


void* hash_obtener(const hash_t* hash, const char* clave) {

    size_t indice = hash_busquda_indice(hash, clave);

    bool pertenece = hash->tabla[indice].estado == O && strcmp(hash->tabla[indice].clave, clave) == 0;

    if (!pertenece) return NULL;

    return hash->tabla[indice].valor;
}


bool hash_pertenece(const hash_t* hash, const char* clave) {
    
    size_t indice = hash_busquda_indice(hash, clave);

    return hash->tabla[indice].estado == O && strcmp(hash->tabla[indice].clave, clave) == 0;
}


size_t hash_cantidad(const hash_t* hash) {
    return hash->ocupados;
}


void hash_destruir(hash_t* hash) {

    for (size_t i = 0; i < hash->capacidad; i++) {
        campo_t campo = hash->tabla[i];

        if (campo.estado == O && hash->destructor) hash->destructor(campo.valor);
        free(campo.clave);
    }

    free(hash->tabla);
    free(hash);
}


// Primitivas Iterador
hash_iter_t* hash_iter_crear(const hash_t* hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));

    if (!iter) return NULL;
    
    iter->hash = hash;
    iter->actual = 0;
    iter->actual = hash_iter_hasta_ocupado(iter);

    return iter;
}


bool hash_iter_avanzar(hash_iter_t* iter) {
    if (hash_iter_al_final(iter)) return false;

    iter->actual++;

    iter->actual = hash_iter_hasta_ocupado(iter);

    return true;
}


const char* hash_iter_ver_actual(const hash_iter_t* iter) {

    if (hash_iter_al_final(iter)) return NULL;

    if (iter->hash->tabla[iter->actual].estado != O) return NULL;

    const char* clave = iter->hash->tabla[iter->actual].clave;

    return clave;
}


bool hash_iter_al_final(const hash_iter_t* iter) {
    return iter->actual == iter->hash->capacidad;
}


void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}
