#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void imprimeDePipe(int leePipe) {
    char buf[100]; // Buffer para recibir el mensaje del padre
    int bytesLeidos;

    // Lee los datos del pipe
    bytesLeidos = read(leePipe, buf, sizeof(buf) - 1);
    if (bytesLeidos > 0) {
        buf[bytesLeidos] = '\0'; // Asegura que el buffer termine con un carácter nulo
        printf("Hijo recibió del pipe: %s\n", buf);
    } else {
        printf("Error leyendo del pipe\n");
    }
    
    close(leePipe); // se cierra el descriptor de lectura
    exit(0); // Se termina el proceso hijo
}

void enviaPipe(int escribePipe) {
    char buf[100];

    // Se solicita que se ingrese una cadena
    printf("Proceso padre, ingresa una cadena de hasta 100 caracteres y enter: \n");
    fgets(buf, sizeof(buf), stdin);

    // Escribe la cadena en el pipe
    write(escribePipe, buf, strlen(buf));
    close(escribePipe); // se Cierra el descriptor de escritura

    // se espera que el proceso hijo termine
    wait(NULL);
    printf("Hijo terminado, terminando proceso padre\n");
    exit(0);
}

int main() {
    pid_t procHijo;
    int pipeFileDescriptors[2]; // los dos decriptores se conectan

    // se crea la pipe
    if (pipe(pipeFileDescriptors) == -1) {
        printf("Error al crear pipe\n");
        exit(1);
    }

    // Creamos el proceso hijo
    procHijo = fork();
    if (procHijo < 0) {
        int errnum = errno; // Preservamos código de error
        printf("Error %d al generar proceso hijo con fork\n", errnum);
        exit(1);
    }

    if (procHijo == 0) {
        // Proceso hijo: cierra pipe de envío y procede
        close(pipeFileDescriptors[1]);
        imprimeDePipe(pipeFileDescriptors[0]);
    }

    if (procHijo > 0) {
        // Proceso padre: cierra pipe de recepción y procede
        close(pipeFileDescriptors[0]);
        enviaPipe(pipeFileDescriptors[1]);
    }

    return 0;
}
