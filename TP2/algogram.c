#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "abb.h"
#include "hash.h"
#include "heap.h"


typedef struct usuario {
    int id_u;
    char* nombre;
    heap_t* feed_u; // ordenados por afinidad y ID
} usuario_t;

typedef struct post {
    int id_p;
    usuario_t* creador; 
    char* mensaje;
    abb_t* likes; // orden alfabetico de nombres
} post_t;

typedef struct red {
    hash_t* usuarios; // "nombre":(id, nombre, posts)
    size_t cant_p;
    post_t** posts;
    usuario_t* logged;
    heap_t* feed;
} red_t;


int usuario_destruir(void* dato);
post_t* post_crear(char* texto, size_t id, usuario_t* creador); // asigna variables y crea abb ordenado por strcmp

// Mensajes de error, para poder modificarlos en caso de ser necesario

char* ERRORES[] = {
    "Error: Ya habia un usuario loggeado\n",
    "Error: usuario no existente\n", // login

    "Error: no habia usuario loggeado\n", // logout, publicar, 

    "Error: Usuario no loggeado o Post inexistente\n" // likear
    "Error: Post inexistente o sin likes\n", // mostrar likes

    "Error: archivo fuente inaccesible\n"};



void escribir_error(char* mensaje) {
    // Envía a stderr un mensaje de error predefinido anteriormente
    fputs(mensaje, stderr);
    fclose(stderr);
}


void leer_e_imprimir(FILE* archivo, int cantidad_columnas) {
    // Lee de a la cantidad de columnas el archivo y lo imprime por pantalla

    //variables correspondientes a la lectura del archivo
    char linea[cantidad_columnas];
    char* cadena = fgets(linea, cantidad_columnas, archivo);

    //variables para la validación de los \n
    int largo_linea;
    bool barraene = false;
    bool linea_completa = false;

    while (cadena) {

        //si la línea anterior estaba completa (se imprimieron todos los 
        //caracteres correspondientes), y yo puse un \n, ese \n es "añadido",
        //entonces si leo un \n inmediatamente después, corresponde a esa linea.

        if (cadena[0] == '\n') {
            if (linea_completa && barraene) {   
                barraene = false; //acá digo que ya el último \n es "real"
            } 
            //si no es "añadido" el \n anterior, entonces el que leo es "nuevo"
            else printf("%s", cadena);
        }

        //este es el caso general
        else printf("%s", cadena); 
        
        largo_linea = (int)strlen(cadena);
        linea_completa = largo_linea == cantidad_columnas - 1;

        if (cadena[largo_linea - 1] != '\n') {
            printf("\n");
            barraene = true;
        }

        cadena = fgets(linea, cantidad_columnas, archivo);
    }
}

bool cargar_usuarios(char* nombre_archivo, hash_t* usuarios) {
    // Lee el archivo y linea a linea crea los usuarios

    bool archivo_al_final = false;
    size_t i = 0;

    while(!archivo_al_final) {
        // Crea el heap y le asigna "i" al usuario
        // Añade al hash el "nombre":usuario
    }



    FILE* usuarios = fopen(nombre_archivo, "r");

    if (!usuarios) {
        escribir_error("Error: archivo fuente inaccesible\n");
        return false;}

    char* linea = NULL;
    size_t largo = 0;
    ssize_t lec;
    lec = getline(&linea, &largo, usuarios);
    while(lec != -1) {
        printf("%s", linea);
        lec = getline(&linea, &largo, usuarios);
    }

    free(linea);
    fclose(usuarios);
    return true;
}

void likear_post(red_t* red, size_t post) {
    if(!red->logged || post > red->cant_p - 1) return;
    abb_guardar(red->posts[post]->likes, red->logged->nombre, NULL);
}

void postear(red_t* red, char* texto) {
    if(!red->logged) return;
    red->posts[red->cant_p] = post_crear(texto, red->cant_p, red->logged);
    red->cant_p ++;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 0;

    char* archivo_usuarios = argv[1];

    red_t* red;

    red->usuarios = hash_crear(usuario_destruir);

    if(!cargar_usuarios(archivo_usuarios, red->usuarios)) return 0;
    // leer_e_imprimir(usuarios, 2);




    return 0;
}
