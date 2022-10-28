#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Mensajes de error, para poder modificarlos en caso de ser necesario
char* ERRORES[] = {
    "Error: Cantidad erronea de parametros\n", 
    "Error: archivo fuente inaccesible\n"};


void escribir_error(int n) {
    // Envía a stderr un mensaje de error predefinido anteriormente

    fputs(ERRORES[n], stderr);
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

bool input_valido(int argc, char* argv[]) {
    // Valida que la cantidad de parámetros recibidos sea la correcta

    if (argc < 2 || argc > 3 || atoi(argv[1]) < 1) {
        escribir_error(0);
        return false;
    }
    
    return true;
}


int main(int argc, char* argv[]) {

    if (!input_valido(argc, argv)) return 0;

    int cantidad_columnas = atoi(argv[1]) + 1;

    if (argc == 2) {
        leer_e_imprimir(stdin, cantidad_columnas);
        return 0;
    }

    FILE* archivo = fopen(argv[2], "r");

    if (!archivo) {
        escribir_error(1);
        return 0;}

    leer_e_imprimir(archivo, cantidad_columnas);
    fclose(archivo);


    return 0;
}
