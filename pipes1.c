//El problema con el codigo es que se tenia un for duplicado como el de la linea 10

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
    int i;
    printf("Ejecutando el programa invocador (execprog1). Sus argumentos son:\n");
    for (i = 0; i < argc; i++) { // el problema era un for duplicado que ya se elimino
        printf("argv[%d]: %s\n", i, argv[i]);
        sleep(10);
        strcpy(argv[0], "execprog2");
        if (execv ("./execprog2", argv) < 0) {
            printf("Error en la invocacion a execprog2\n");
            exit(1);
        };
    }

    exit(0);
}
