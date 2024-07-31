#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
    // Creamos socket
    int s = socket(AF_INET, SOCK_STREAM, 0);

    // Cargamos datos de IP:PORT del server
    /*Cuando hacemos bind, podemos especificar la ip particular o hacer que escuche cualquiera
    en este caso asignamos especificamenta la loopback.*/
    struct sockaddr_in serveraddr;// Creo la address
    bzero((char *)&serveraddr, sizeof(serveraddr)); // hacemos 0 la address o podriamos hacer = {0} al crearla
    serveraddr.sin_family = AF_INET; // tipo
    serveraddr.sin_port = htons(4096); // puerto > 1024 para evitar errores de permiso 
    if (inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr)) <= 0) { // asignamos en addr la ip
        fprintf(stderr, "ERROR invalid server IP\n");
        exit(EXIT_FAILURE);
    }

    // Asignamos ip y puerto con bind()
    if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) { // el size off pasamos el tamaño
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Seteamos socket en modo Listening
    if (listen(s, 1) == -1) { // backlog=10, nos da que podemos tener hasta 10 clientes si fuera 10 en vez de 1
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) { // para aceptar conexiones de multiples clientes.
        // Ejecutamos accept() para recibir conexiones entrantes
        socklen_t addr_len = sizeof(struct sockaddr_in); // es parametro de entrada y salida
        struct sockaddr_in clientaddr;
        int newfd; // este es el socket nuevo que devolvera
        printf("server: esperando una conexion...\n");
        if ((newfd = accept(s, (struct sockaddr *)&clientaddr, &addr_len)) == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char ipClient[32];
        inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient)); // convierte a la notacion ip
        printf("server: conexion desde:  %s\n", ipClient);

        // prueba cierre de conexion. El cliente recibira SIGPIPE. 
        /*
        printf("presionar enter para salir\n");
        getchar();
        close(newfd);
        close(s);
        exit(EXIT_FAILURE);
        */

        // Leemos mensaje de cliente
        char buffer[128];
        int n;
        if ((n = read(newfd, buffer, 128)) == -1) { 
            // la tipica forma de leer un fd, si el cliente cierra la conexion da 0 !! hay que agregarlo
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer[n] = 0x00; // agregamos el '/0'
        printf("server: recibi %d bytes:%s\n", n, buffer);

        // Enviamos mensaje a cliente
        if ((n = write(newfd, "chau", 5)) == -1) { // respondemos
            perror("write");
            exit(EXIT_FAILURE);
        }
        printf("server: envie %d bytes\n", n);

        // Esperamos a que el cliente cierre
        printf("server: esperando a que el cliente cierre la conexion...\n");
        if ((n = read(newfd, buffer, 128)) == -1) { // aca se que va a  cerrar la conexion
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (n != 0) {
            // No deberia ocurrir ya que si el cliente cerro la conexion, read devuelve 0
            buffer[n] = 0x00;
            printf("server: recibi %d bytes:%s\n", n, buffer);
        }

        // Cerramos conexion con cliente
        close(newfd); // como cerró, cierro la conexion
    }

    return EXIT_SUCCESS;
}
