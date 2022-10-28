#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

/* AUXILIAR */


void borrar(void* puntero) {
    free(puntero);
    printf("Elemento eliminado\n");
}

//suma todos los elementos de la lista
bool sumar(void* dato, void* extra) {
    *(int*)extra += *(int*)dato ;
    return true ;
}

//suma 1 a los primeros 3 elementos de la lista
bool sumar_a_primeros_tres(void* dato, void* extra) {
    if (*(int*)extra == 3) {
        return false;
    }
    *(int*)extra += 1;
    *(int*)dato += 1;
    return true ;
}

//suma el doble de los elementos pares de la lista
bool doble_de_pares(void* dato, void* extra) {
    if ((*(int*)dato) % 2 == 0) {
        *(int*)extra += 2*(*(int*)dato) ;
    }
    return true;
}

void recien_creada(lista_t *lista) {
    print_test("la lista esta vacía", lista_esta_vacia(lista) == true);
    print_test("borrar primero es inválido", lista_borrar_primero(lista)==NULL);
    print_test("ver_primero es inválido", lista_ver_primero(lista)==NULL);
    print_test("ver_ultimo es inválido", lista_ver_ultimo(lista)==NULL);
    print_test("el largo de la lista es 0", lista_largo(lista)==0);
}


/* PRUEBAS */

void pruebas_lista_vacia(void) {

    printf("\nINICIO DE PRUEBAS CON LISTA VACÍA\n");

    lista_t *lista = lista_crear();
    
    recien_creada(lista) ;

    lista_destruir(lista, NULL) ;
}

void pruebas_generales_lista(void) {

    printf("\nINICIO DE PRUEBAS GENERALES DE LA LISTA\n");

    lista_t *lista = lista_crear();

    int valor1 = 1 ;
    int valor2 = 2 ;
    int valor3 = 3 ;

    recien_creada(lista) ;

    print_test("Insertar un elemento en la lista", lista_insertar_primero(lista, &valor1) == true);
    print_test("El primer elemento de la lista es el que se agregó", lista_ver_primero(lista) == &valor1);
    print_test("El último elemento de la lista es el que se agregó", lista_ver_ultimo(lista) == &valor1);
    print_test("Insertar un elemento en la lista despues del primero", lista_insertar_ultimo(lista, &valor2) == true);
    print_test("El último elemento de la lista es el que se agregó", lista_ver_ultimo(lista) == &valor2);
    print_test("Insertar un elemento en la lista antes del primero", lista_insertar_primero(lista, &valor3) == true);
    print_test("El primer elemento de la lista es el que se agregó", lista_ver_primero(lista) == &valor3);
    print_test("La lista no está vacía", lista_esta_vacia(lista) == false);
    print_test("El largo de la lista es igual a la cantidad de elementos agregados", lista_largo(lista) == 3);
    print_test("Se elimina el primer elemento", lista_borrar_primero(lista) == &valor3);
    print_test("El segundo elemento de la lista pasa a ser el primero", lista_ver_primero(lista) == &valor1);
    print_test("Se elimina el segundo elemento", lista_borrar_primero(lista) == &valor1);
    print_test("El ultimo elemento de la lista pasa a ser el primero", lista_ver_primero(lista) == &valor2); //primero
    print_test("Se elimina el último elemento", lista_borrar_primero(lista) == &valor2);
    print_test("La lista está vacía", lista_esta_vacia(lista) == true);

    printf("VERIFICAR QUE LISTA VACIADA SE COMPORTA COMO RECIEN CREADA\n");
    
    recien_creada(lista) ;

    lista_destruir(lista, NULL) ;
}

void pruebas_lista_volumen(void) {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

    int tam = 10;
    int array_aux[tam] ;
    int i;

    lista_t *lista_vol = lista_crear();

    print_test("La lista esta vacía", lista_esta_vacia(lista_vol) == true);

    bool ok = true;
    for (i = 0; i < tam; i++) {
        array_aux[i] = i ;

        ok = lista_insertar_ultimo(lista_vol, &array_aux[i]) ;

        ok = (lista_ver_primero(lista_vol) == &array_aux[0]) ;
    }
    print_test("Se pudieron insertar todos los elementos", ok);

    ok = true;
    for (i = 0; i < tam; i++) {

        int* aux = (int*)lista_borrar_primero(lista_vol) ;

        ok = *aux == array_aux[i] ;
    }

    print_test("Se pudieron borrar todos los elementos", ok);

    recien_creada(lista_vol) ;

    lista_destruir(lista_vol, NULL) ;
}

void pruebas_destruccion_lista_vacia(void) {
    
    printf("\nINICIO DE PRUEBAS DE DESTRUCCIÓN CON LISTA VACÍA\n");

    printf("Se crea una nueva lista vacía\n");

    lista_t *lista_1 = lista_crear();

    lista_destruir(lista_1, NULL) ;

    printf("La lista vacía se destruyó correctamente usando NULL\n") ;
    
    printf("se crea una nueva lista vacía\n");

    lista_t *lista_2 = lista_crear();

    lista_destruir(lista_2, borrar) ;

    printf("La lista vacía se destruyó correctamente usando una función\n");
}

void pruebas_destruccion_lista_no_vacia(void) {

    printf("\nINICIO DE PRUEBAS DE DESTRUCCIÓN CON LISTA NO VACÍA\n");
    
    printf("Se crea una lista vacía\n");

    lista_t *lista_1 = lista_crear();

    int num1 = 1 ;
    int num2 = 2 ;

    printf("Se agregan números enteros\n");

    lista_insertar_ultimo(lista_1, &num1) ;
    lista_insertar_ultimo(lista_1, &num2) ;
    
    lista_destruir(lista_1, NULL) ;

    printf("La lista de enteros se destruyó correctamente usando NULL\n") ;
    

    printf("Se crea una nueva lista vacía\n");

    lista_t *lista_2 = lista_crear();

    int *primero = malloc(sizeof(int*)) ;
    int *segundo = malloc(sizeof(int*)) ;

    printf("Se agregan punteros a entero\n");

    lista_insertar_ultimo(lista_2, primero) ;
    lista_insertar_ultimo(lista_2, segundo) ;

    lista_destruir(lista_2, borrar) ;

    printf("La lista de punteros se destruyó correctamente usando una función\n");
    
}

void pruebas_iterador_interno(void) {

    printf("\nINICIO DE PRUEBAS CON ITERADOR INTERNO\n");

    printf("Se crea una lista vacía\n");

    lista_t *lista = lista_crear();

    recien_creada(lista);

    printf("Se le agregan enteros\n");

    int numeros[] = {1,2,3,4,5,6} ;
    int suma = 21;
    int doble_pares = 24;
    int iteraciones = 3;
    int extra_suma = 0;
    int extra_suma_doble_pares = 0;
    int extra_cont = 0;

    
    lista_insertar_ultimo(lista, &numeros[0]);
    lista_insertar_ultimo(lista, &numeros[1]);
    lista_insertar_ultimo(lista, &numeros[2]);
    lista_insertar_ultimo(lista, &numeros[3]);
    lista_insertar_ultimo(lista, &numeros[4]);
    lista_insertar_ultimo(lista, &numeros[5]);

    printf("Pruebas sin corte del iterador interno\n");

    lista_iterar(lista, sumar, &extra_suma);   
    print_test("Todos los elementos se sumaron correctamente", extra_suma == suma);

    lista_iterar(lista, doble_de_pares, &extra_suma_doble_pares);
    print_test("Todos los elementos pares se sumaron correctamente", extra_suma_doble_pares == doble_pares);  

    printf("Pruebas con corte del iterador interno\n");

    lista_iterar(lista, sumar_a_primeros_tres, &extra_cont);
    print_test("La iteración no llegó al final", extra_cont == iteraciones);

    lista_destruir(lista, NULL);
}

void pruebas_iterador_externo(void) {

    printf("\nINICIO DE PRUEBAS CON ITERADOR EXTERNO\n");

    printf("Se crea una lista vacía y su iterador\n");

    int numeros[] = {1,2,3} ;

    lista_t *lista = lista_crear();

    lista_iter_t *iter = lista_iter_crear(lista);
    
    recien_creada(lista);

    print_test("Se puede insertar al principio de la lista", lista_iter_insertar(iter, &numeros[0]) == true);
    print_test("Se puede avanzar el iterador", lista_iter_avanzar(iter) == true);
    print_test("El iterador está al final de la lista", lista_iter_al_final(iter) == true);
    print_test("Se puede insertar cuando el iterador está al final de la lista", lista_iter_insertar(iter, &numeros[1]) == true);
    print_test("El actual es el ultimo agregado", lista_iter_ver_actual(iter) == &numeros[1]);
    print_test("Se puede insertar en el medio", lista_iter_insertar(iter, &numeros[2]) == true);
    print_test("El actual es el ultimo agregado", lista_iter_ver_actual(iter) == &numeros[2]);
    print_test("Se puede borrar en el medio", lista_iter_borrar(iter) == &numeros[2]);
    print_test("El actual es el elemento que le sigue al eliminado", lista_iter_ver_actual(iter) == &numeros[1]);
    print_test("El actual es el último elemento", lista_ver_ultimo(lista) == &numeros[1]);
    print_test("Se puede borrar al final", lista_iter_borrar(iter) == &numeros[1]);
    print_test("Se modificó el último", lista_ver_ultimo(lista) == &numeros[0]);
    print_test("El iterador está al final de la lista", lista_iter_al_final(iter) == true);
    print_test("No se puede avanzar", lista_iter_avanzar(iter) == false);
    
    lista_iter_destruir(iter) ;
    lista_destruir(lista, NULL) ;

    printf("La lista y su iterador se destruyeron correctamente\n");

    
    printf("Se crea otra lista vacía\n");    

    lista_t *lista2 = lista_crear();
    recien_creada(lista2);

    printf("Se agregan enteros\n");    

    lista_insertar_ultimo(lista2, &numeros[0]);
    lista_insertar_ultimo(lista2, &numeros[1]);
    lista_insertar_ultimo(lista2, &numeros[2]);

    printf("Se crea un nuevo iterador\n");

    lista_iter_t *iter2 = lista_iter_crear(lista2);
    
    print_test("El primer elemento de la lista es el primero que se agregó", lista_ver_primero(lista2) == &numeros[0]);
    print_test("El actual es el primero de la lista", lista_iter_ver_actual(iter2) == &numeros[0]);
    print_test("Se puede borrar el primer elemento", lista_iter_borrar(iter2) == &numeros[0]);
    print_test("El primer elemento de la lista se modificó", lista_ver_primero(lista2) == &numeros[1]);

    
    lista_iter_destruir(iter2);
    lista_destruir(lista2, NULL) ;

    printf("La lista y su iterador se destruyeron correctamente\n");
}


void pruebas_lista_estudiante() {
    
    pruebas_lista_vacia();
    pruebas_generales_lista();
    pruebas_lista_volumen();
    pruebas_destruccion_lista_vacia();
    pruebas_destruccion_lista_no_vacia();
    pruebas_iterador_interno();
    pruebas_iterador_externo();
    
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif