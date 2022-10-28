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
    int caracter_leido = 1;

    while (caracter_leido != EOF) {

        for (int i = 0; i < cantidad_columnas; i++) {
            caracter_leido = fgetc(archivo);

            if (caracter_leido == EOF) return;
            printf("%c", caracter_leido);

            if (caracter_leido == '\n') break;
        }

        if (caracter_leido != '\n') printf("\n");
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

    int cantidad_columnas = atoi(argv[1]);

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


    printf("\n");
    return 0;
}
