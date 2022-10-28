#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

//FUNCIONES AUXILIARES
int comparar(const void* numero1, const void* numero2){
    return (*((int*)numero1) - *((int*)numero2));
}

void print_arreglo(void** arr, size_t cant) {
    printf("--EXT: ");
    for(size_t i = 0; i < cant; i++) {
        printf("%d ", *(int*)arr[i]);
    }
    printf("\n");
}


void mezclar_valores(int datos[], size_t largo){
    srand((unsigned int)time(NULL));

    for (int i = 0; i < (int)largo - 1; i++){
        int j = rand() % (int)largo;
        int aux = datos[i];
        datos[i] = datos[j];
        datos[j] = aux;
    }
}

void pruebas_heap_vacio(){
    printf("PRUEBAS CREAR HEAP Y GENERALES:\n");

    heap_t *heap = heap_crear(comparar);

    print_test("Prueba1: El heap fue creado", heap);
    print_test("Prueba2: El heap se encuentra vacio", heap_esta_vacio(heap));
    print_test("Prueba3: La cantidad de elementos es la correcta", heap_cantidad(heap) == 0);
    print_test("Prueba4: Ver maximo devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba5: Desencolar devuelve NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
    printf("\n");
}

void pruebas_heap_encolar(){
    printf("PRUEBAS ENCOLAR ELEMENTOS:\n");
    
    heap_t *heap = heap_crear(comparar);
    int numeros[] = {35, 400, 21, 1};
    
    // primer valor
    print_test("Prueba1: Encolar primer valor", heap_encolar(heap, &numeros[0]));
    print_test("Prueba2: El heap no se encuentra vacio", !heap_esta_vacio(heap));
    print_test("Prueba3: Ver max devuelve el valor correcto", heap_ver_max(heap) == &numeros[0]);

    // segundo valor
    print_test("Prueba4: Encolar segundo valor", heap_encolar(heap, &numeros[1]));
    print_test("Prueba5: Ver max devuelve el valor correcto", heap_ver_max(heap) == &numeros[1]);

    // tercer valor
    print_test("Prueba6: Encolar tercer valor", heap_encolar(heap, &numeros[2]));
    print_test("Prueba7: Ver max devuelve el elemento correcto", *(int *)heap_ver_max(heap) == numeros[1]);
    // cuarto valor
    print_test("Prueba8: Encolar cuarto valor", heap_encolar(heap, &numeros[3]));
    print_test("Prueba9: La cantidad de elementos es la correcta", heap_cantidad(heap) == 4);
    print_test("Prueba10: Ver max devuelve el valor correcto", heap_ver_max(heap) == &numeros[1]);
    print_test("Prueba11: El heap no se encuentra vacio", !heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
    printf("\n");
}

void pruebas_heap_desencolar(){
    printf("PRUEBAS DESENCOLAR ELEMENTOS:\n");
    
    heap_t *heap = heap_crear(comparar);
    int numeros[] = {35, 400, 21, 1};

    heap_encolar(heap, &numeros[0]);
    heap_encolar(heap, &numeros[1]);
    heap_encolar(heap, &numeros[2]);
    heap_encolar(heap, &numeros[3]);

    // desencolar 1 vez
    print_test("Prueba1: Desencolar devuelve el valor correcto", heap_desencolar(heap) == &numeros[1]);
    print_test("Prueba2: Ver max devuelve el valor correcto", heap_ver_max(heap) == &numeros[0]);
    print_test("Prueba3: La cantidad de elementos es la correcta", heap_cantidad(heap) == 3);

    // desencolar 2 veces
    print_test("Prueba4: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[0]);
    print_test("Prueba5: Ver max devuelve el elemento correcto", heap_ver_max(heap) == &numeros[2]);
    print_test("Prueba6: La cantidad de elementos es la correcta", heap_cantidad(heap) == 2);

    // desencolar 3 veces
    print_test("Prueba7: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[2]);
    print_test("Prueba8: Ver max devuelve el elemento correcto", heap_ver_max(heap) == &numeros[3]);
    print_test("Prueba9: La cantidad de elementos es la correcta", heap_cantidad(heap) == 1);

    // desencolar y el heap queda vacio
    print_test("Prueba10: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[3]);
    print_test("Prueba11: La cantidad de elementos es la correcta", heap_cantidad(heap) == 0);
    print_test("Prueba12: El heap se encuentra vacio", heap_esta_vacio(heap));
    print_test("Prueba13: Desencolar elemento devuelve NULL", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
    printf("\n");

}

void pruebas_heap_volumen(size_t largo){
    printf("PRUEBAS HEAP VOLUMEN:\n");
    heap_t *heap = heap_crear(comparar);

    int valores[largo];

    for (int i = 0; i < largo; i++){
        valores[i] = i;
    }

    int maximo = valores[largo - 1];
    mezclar_valores(valores, largo);

    bool ok = true;
    for (size_t i = 0; i < largo; i++){
        ok = heap_encolar(heap, &valores[i]);
        if (!ok){
            break;
        }
    }

    print_test("Prueba1: El heap no se encuentra vacio", !heap_esta_vacio(heap));
    print_test("Prueba2: Todos los elementos fueron encolados", ok);
    print_test("Prueba3: La cantidad de elementos es la correcta", heap_cantidad(heap) == largo);
    print_test("Prueba4: Ver maximo devuelve el valor correcto", *(int*)heap_ver_max(heap) == maximo);


    bool chequeo = true;
    while(!heap_esta_vacio(heap)){
        print_arreglo(devolver_arreglo(heap), heap_cantidad(heap));
        int desencolado = *(int*)heap_desencolar(heap);
        chequeo = desencolado == maximo;
        maximo--;
        if (!chequeo){
            break;
            printf("Error da %d / no %d\n", desencolado, maximo);
        }
    }

    print_test("Prueba5: Todos los elementos fueron desencolados", chequeo);
    print_test("Prueba6: La cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba7: El heap se encuentra vacio", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
    printf("\n");
}

void pruebas_heap_destruccion_null(){
    printf("PRUEBAS HEAP DESTRUCCION NULL:\n");
    heap_t* heap = heap_crear(comparar);
    size_t arreglo[] = {3, 25, 12, 2, 4, 71, 10, 8, 14};

    bool ok = true;
    for(int i = 0; i < 9; i++){
        ok = heap_encolar(heap, &arreglo[i]);
        if(!ok) break;
    }

    print_test("Prueba1: Se encolaron elementos estaticos", ok);
    
    heap_destruir(heap, NULL);
    printf("\n");
}

void pruebas_heap_destruccion_no_null(){
    printf("PRUEBAS DE DESTRUCCION NO NULL\n");
    heap_t *heap = heap_crear(comparar);
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    int *d = malloc(sizeof(int));
    *a = 1;
    *b = 20;
    *c = 10;
    *d = 2; 

    // pruebas de insertar elementos dinamicos
    heap_encolar(heap, a);
    heap_encolar(heap, b);
    heap_encolar(heap, c);
    heap_encolar(heap, d);
    print_test("Prueba1: Se encolaron algunos elementos dinamicos", heap_cantidad(heap) == 4);
    heap_destruir(heap, free);
    printf("\n");
}

void pruebas_heap_arr(){
    printf("PRUEBAS HEAP ARREGLO\n");

    int numeros[] = {35, 400, 21, 1};
    size_t largo = 4; 
    size_t capacidad = 10;

    void** arr_numeros = malloc(sizeof(void*)*capacidad);

    for(int i = 0; i < largo; i++){
        arr_numeros[i] = &numeros[i];
    }

    heap_t* heap = heap_crear_arr(arr_numeros, largo, comparar);

    print_test("Prueba1: El heap fue creado", heap);
    print_test("Prueba2: El heap no se encuentra vacio", !heap_esta_vacio(heap));
    print_test("Prueba3: La cantidad de elementos es la correcta", heap_cantidad(heap) == largo);

    print_test("Prueba4: Ver maximo devuelve el valor correcto", heap_ver_max(heap) == &numeros[1]);
    print_test("Prueba5: Desencolar devuelve el valor correcto", *(int*)heap_desencolar(heap) == numeros[1]);
    print_test("Prueba6: Ver max devuelve el valor correcto", heap_ver_max(heap) == &numeros[0]);
    print_test("Prueba7: La cantidad de elementos es la correcta", heap_cantidad(heap) == 3);

    // desencolar 2 veces
    print_test("Prueba8: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[0]);
    print_test("Prueba9: Ver max devuelve el elemento correcto", heap_ver_max(heap) == &numeros[2]);
    print_test("Prueba10: La cantidad de elementos es la correcta", heap_cantidad(heap) == 2);

    // desencolar 3 veces
    print_test("Prueba11: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[2]);
    print_test("Prueba12: Ver max devuelve el elemento correcto", heap_ver_max(heap) == &numeros[3]);
    print_test("Prueba13: La cantidad de elementos es la correcta", heap_cantidad(heap) == 1);

    // desencolar y el heap queda vacio
    print_test("Prueba14: Desencolar devuelve el elemento correcto", heap_desencolar(heap) == &numeros[3]);
    print_test("Prueba15: La cantidad de elementos es la correcta", heap_cantidad(heap) == 0);
    print_test("Prueba16: El heap se encuentra vacio", heap_esta_vacio(heap));
    print_test("Prueba17: Desencolar elemento devuelve NULL", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
    free(arr_numeros);
    printf("\n");
}

void pruebas_heapsort(){
    printf("PRUEBAS HEAPSORT\n");
    
    size_t arreglo[] = {4, 7, 8, 1, 0, 2, 5, 6, 3, 9};
    size_t arreglo_ordenado[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};


    size_t n = sizeof(arreglo) / sizeof(arreglo[0]);
    size_t capacidad = 15;


    void** arr = malloc(sizeof(void*)*capacidad);
    if(!arr) return;

    for(size_t i = 0; i < n; i++){
        arr[i] = &arreglo[i];
    }


    heap_sort(arr, n, comparar); // ordenamos el arreglo


    bool ok = true;
    for(int i = 0; i < n; i++){
        ok = arreglo_ordenado[i] == *((int*)arr[i]);
        if(!ok) break;
    }
    
    print_test("Prueba1: El arreglo de numeros se ordeno de menor a mayor", ok);
    free(arr);
    printf("\n");
}


void pruebas_heap_estudiante(){
    // pruebas_heapsort();
    // pruebas_heap_vacio();
    // pruebas_heap_encolar();
    // pruebas_heap_destruccion_null();
    // pruebas_heap_destruccion_no_null();
    // pruebas_heap_desencolar();
    // pruebas_heap_arr();


    pruebas_heap_volumen(10);
}

#ifndef CORRECTOR

int main(){
    pruebas_heap_estudiante();
    return failure_count() > 0;
}

#endif // CORRECTOR
