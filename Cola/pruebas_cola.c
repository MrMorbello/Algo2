#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void prueba_cola_vacia(void) {
    cola_t* cola = cola_crear();
    int valor1 = 2;

    printf("\nINICIO DE PRUEBAS DE COLA VACIA\n");

    print_test("Crear cola vacía", cola_esta_vacia(cola));
    print_test("Ver primero cola vacía", cola_ver_primero(cola) == NULL);
    print_test("Desencolar cola vacía", cola_desencolar(cola) == NULL);
    print_test("Encolar un elemento", cola_encolar(cola, &valor1));

    printf("\n");
    cola_destruir(cola, NULL);
}

void pruebas_cola_dos_elementos(void) {
    cola_t* cola = cola_crear();
    int v1 = 1;
    int v2 = 2;
    int vnull;

    printf("\nINICIO DE PRUEBAS DE DOS ELEMENTOS\n");

    //Ecolar 1
    print_test("Creada cola vacía", cola_esta_vacia(cola));
    print_test("Encolar un elemento", cola_encolar(cola, &v1));
    print_test("Ver primero cola 1 elemento", cola_ver_primero(cola) == &v1);

    //Ecolar 2 y desencolar 1
    print_test("Encolar dos elementos", cola_encolar(cola, &v2));
    print_test("Ver primero cola dos elementos", cola_ver_primero(cola) == &v1);
    print_test("Desencolar un elemento de dos", cola_desencolar(cola) == &v1);
    print_test("Ver primero con un elemento", cola_ver_primero(cola) == &v2);

    //Ecolar con null
    print_test("Encolar nuevamente otro elemento (NULL)", cola_encolar(cola, &vnull));
    print_test("Ver primero con elemento null", cola_ver_primero(cola) == &v2);
    print_test("Desencolar elemento NULL", cola_desencolar(cola) == &v2);

    //Termino de vaciar
    print_test("Desapilar un elemento de uno", cola_desencolar(cola) == &vnull);
    print_test("Ver tope vacio", cola_ver_primero(cola) == NULL);
    print_test("Comprobar que cola esta vacía", cola_esta_vacia(cola));

    printf("\n");
    cola_destruir(cola, NULL);
}

void pruebas_cola_extremo_elementos(void) {
    cola_t* cola = cola_crear();
    int largo = 10000;
    char* vector[largo];

    printf("\nINICIO DE PRUEBAS DE MUCHOS ELEMENTOS\n");

    //Encolo un elemento para tenerlo como referencia más tarde.

    //Encolo y desencolo N elementos
    for (size_t i = 0; i < largo; i++) {
        cola_encolar(cola, vector[i]);}
    
    cola_encolar(cola, &largo);

    for (size_t i = 0; i < largo; i++) {
        cola_desencolar(cola);}

    //Verifico que esté el elemento que encolé último en el primero
    print_test("Pruebas completas con n elementos", cola_ver_primero(cola) == &largo);

    printf("\n");

    cola_destruir(cola, NULL);
}

void borrar(void* puntero) {
    free(puntero);
    printf("Elemento eliminado\n");
}

void prueba_memoria_dinamica(void) {
    
    cola_t* cola = cola_crear();
    int* v1 = malloc(sizeof(int));
    int* v2 = malloc(sizeof(int));

    printf("\nINICIO DE PRUEBAS DE MEMORIA DINAMICA\n");

    print_test("Existen los dos elementos", v1 && v2);
    print_test("Creada cola vacía", cola_esta_vacia(cola));
    print_test("Encolar primer elemento", cola_encolar(cola, v1));
    print_test("Encolar segundo elementos", cola_encolar(cola, v2));

    cola_destruir(cola, borrar);

    printf("\n");

}

void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    pruebas_cola_dos_elementos();
    pruebas_cola_extremo_elementos();
    prueba_memoria_dinamica();
}


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
