#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>


typedef struct abb abb_t;

// Primitivas ABB

// Función para comparar las claves.
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// Función para destruir datos.
typedef void (*abb_destruir_dato_t) (void *);

// Se crea el árbol.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un par (clave,dato) en el árbol. Si la clave ya existe, elimina su dato y lo reemplaza.
 * De no poder guardarlo devuelve false, caso contrario devuelve true.
 * Pre: La estructura abb fue inicializada.
 * Post: Se almacenó el par (clave, dato).
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado. Devuelve
 * NULL si la clave no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se devolvió el dato,
 * en caso de que se lo encontrara.
 */
void *abb_borrar(abb_t *arbol, const char *clave);


/* Obtiene el dato de un elemento(par clave,valor) del abb, si la 
 * clave no se encuentra devuelve NULL.
 * Pre: La estructura hash fue inicializada.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada.
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void abb_destruir(abb_t *arbol);

/* Primitiva del iterador interno
 * Aplica a todos los elementos del abb la función visitar
 * Pre: La estructura abb fue creada, visitar es una función de la firma dada o NULL.
 * Post: Se aplicó la función visitar a los elementos hasta que devuelve false.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


// Primitivas Iter ABB
typedef struct abb_iter abb_iter_t;

// Crea el iterador del abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza el iterador.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Comprueba si terminó la iteración.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
void abb_iter_in_destruir(abb_iter_t* iter);


#endif  // ABB_H