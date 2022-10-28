#include "tp0.h"
#include <stdio.h>

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}


int maximo(int vector[], int n) {
    //posición de máximo del arreglo
    if(n == 0) return -1;
    int max = vector[0];
    int indice = 0;
    for(int i = 0; i < n; i++){
        if(vector[i] > max){
            max = vector[i];
            indice = i;
        }
    }
    return indice;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    // 1<2(-1)  =(0) 1>2(1)
    // El primer elemento en el que difieren, es menor o no existe.

    //indice máximo que tienen en común los dos (entre 4 y 5, es 4)
    int max_indice_compartido = n2 > n1 ? n1 : n2;

    //recorrido de casos "normales" de diferencia
    for(int i = 0; i < max_indice_compartido; i++){
        if (vector1[i] == vector2[i]) continue;
        if (vector1[i] < vector2[i]) return -1;
        return 1;
    }

    //en caso de terminar porque no se llegó a la diferencia, pueden ser != largo
    if(n1 != n2) return n1 < n2 ? -1 : 1;

    return 0;
}

int buscar_minimo_posicion(int vector[], int desde, int hasta){
    if(desde >= hasta){
        return desde;
    }
    int min = vector[desde];
    int posicion = desde;
    for(int i = desde; i <= hasta; i++){
        if(vector[i] < min){
            min =  vector[i];
            posicion = i;
        }
    }
    return posicion;
}

void seleccion(int vector[], int n) {
    // itera el largo del arreglo
    for(int i = 0; i < n; i++){

        int p = buscar_minimo_posicion(vector, i, n - 1);
        swap(&vector[i], &vector[p]);
    }
}
