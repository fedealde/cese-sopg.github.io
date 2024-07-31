#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

//Con netstat -plnt nos da la conexion que tenemos, en este caso 4096 le llama bre, bridge relay element
//Vemos que al cliente le da un puerto efimero

int main(void) {
    // Creamos socket
    int s = socket(PF_INET, SOCK_STREAM, 0);

    // Cargamos datos de direccion de server
    struct sockaddr_in serveraddr;
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4096);
    if (inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr)) <= 0) {
        fprintf(stderr, "ERROR invalid server IP\n");
        exit(EXIT_FAILURE);
    }

    // Ejecutamos connect()
    if (connect(s, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        fprintf(stderr, "ERROR connecting\n");
        close(s);
        exit(EXIT_FAILURE);
    }

    // Enviamos mensaje a server
    printf("Ingrese un mensaje:\n");
    char buf[128];
    fgets(buf, sizeof(buf), stdin);
    int n = write(s, buf, strlen(buf));
    printf("cliente: envie %d bytes\n", n);
    // prueba server desconectado. Al enviar por segunda vez se recibe SIGPIPE
    // n = write(s, buf, strlen(buf));
    //__________________________

    if (n <= 0) {
        fprintf(stderr, "ERROR sending command\n");
        close(s);
        exit(EXIT_FAILURE);
    }

    // Leemos respuesta de server
    n = read(s, buf, sizeof(buf));
    printf("cliente: recibi %d bytes\n", n);
    if (n > 0) {
        buf[n] = 0x00;
        printf("cliente: recibi: '%s'\n", buf);
    }
    close(s);//si no estuviera, se cierra igualmente, pero es buena practica

/*si ejecuto multiples clientes, todos estan con el handshake hecho, pero el server solo 
contesta al primero, luego al segundo lo hace. Puede antender tantos como dice el backlog.
El kernel a veces asigna un poco mas, pero en algun momento pone freno y hace esperar
*/

    return EXIT_SUCCESS;
}
