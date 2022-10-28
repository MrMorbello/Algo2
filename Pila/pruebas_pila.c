#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>


static void prueba_pila_vacia(void) {
    pila_t* pila = pila_crear();
    int valor1 = 2;

    printf("\nINICIO DE PRUEBAS DE PILA VACIA\n");

    print_test("Creada pila vacía", pila_esta_vacia(pila));
    print_test("Ver tope pila vacía", pila_ver_tope(pila) == NULL);
    print_test("Desapilar pila vacía", pila_desapilar(pila) == NULL);
    print_test("Apilar un elemento", pila_apilar(pila, &valor1));

    printf("\n");
    pila_destruir(pila);
}

static void pruebas_pila_dos_elementos(void) {
    pila_t* pila = pila_crear();
    int v1 = 1;
    int v2 = 2;
    int vnull;

    printf("\nINICIO DE PRUEBAS DE DOS ELEMENTOS\n");

    //Apilo 1
    print_test("Creada pila vacía", pila_esta_vacia(pila));
    print_test("Apilar un elemento", pila_apilar(pila, &v1));
    print_test("Ver tope pila 1 elemento", pila_ver_tope(pila) == &v1);

    //Apilo 2
    print_test("Apilar dos elementos", pila_apilar(pila, &v2));
    print_test("Ver tope pila dos elementos", pila_ver_tope(pila) == &v2);
    print_test("Desapilar un elemento de dos", pila_desapilar(pila) == &v2);
    print_test("Ver tope con un elemento", pila_ver_tope(pila) == &v1);

    //Reapilo con otro
    print_test("Apilar nuevamente otro elemento (NULL)", pila_apilar(pila, &vnull));
    print_test("Ver tope con elemento null", pila_ver_tope(pila) == &vnull);
    print_test("Desapilar elemento NULL", pila_desapilar(pila) == &vnull);

    //Termino de vaciar
    print_test("Desapilar un elemento de uno", pila_desapilar(pila) == &v1);
    print_test("Ver tope vacio", pila_ver_tope(pila) == NULL);
    print_test("Comprobar que pila esta vacía", pila_esta_vacia(pila));

    printf("\n");
    pila_destruir(pila);
}

void pruebas_pila_extremo_elementos(void) {
    pila_t* pila = pila_crear();
    int largo = 10000;
    char* vector[largo];

    printf("\nINICIO DE PRUEBAS DE MUCHOS ELEMENTOS\n");

    //Apilo un elemento para tenerlo como referencia más tarde.
    pila_apilar(pila, &largo);

    //Apilo y desapilo N elementos
    for (size_t i = 0; i < largo; i++) {
        pila_apilar(pila, vector[i]);}

    for (size_t i = 0; i < largo; i++) {
        pila_desapilar(pila);}

    //Reviso que esté el primer elemento que apilé en el tope.
    print_test("Pruebas completas con n elementos", pila_ver_tope(pila) == &largo);

    printf("\n");
    pila_destruir(pila);
}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    pruebas_pila_dos_elementos();
    pruebas_pila_extremo_elementos();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
