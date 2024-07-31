#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    // Creamos socket
    int s = socket(AF_INET, SOCK_DGRAM, 0);

    // Cargamos datos de IP:PORT del server
    struct sockaddr_in serveraddr;
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4096);
    serveraddr.sin_addr.s_addr =
        INADDR_ANY; // if(inet_pton(AF_INET, "127.0.0.1", 
                    // &(serveraddr.sin_addr))<=0)
    //acptamos conexiones desde cualquier interfaz


    // Abrimos puerto con bind()
    if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        close(s);
        perror("listener: bind");
    }

    // Escuchamos datagram con recvfrom()
    struct sockaddr_in clientaddr;
    bzero((char *)&clientaddr, sizeof(clientaddr));
    socklen_t addr_len = sizeof(clientaddr);

//NO HACEMOS NI LISTEN NI ACCEPT PORQUE NO ES ORIENTADO A CONEXION
// Por lo que tenemos un solo socket, no uno para listen y uno por cada cliente
// Aca uno solo escucha todos

    char buffer[128];
    /*Parecido a read pero me agrega que no solo recibe lo que llego del socket, sino que
    dice de quien vino el mensaje ya que puede atender a multiples clientes*/
    int numBytes = recvfrom(s, buffer, 127, 0,
                            (struct sockaddr *)&clientaddr, &addr_len);
    printf("Se recibieron %d bytes", numBytes);

    char ipClient[32];
    inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
    int port = ntohs(clientaddr.sin_port);
    printf(" desde ip:%s port:%d", ipClient, port);

    buffer[numBytes] = 0;
    printf("\nMSG:%s\n", buffer);

    close(s);

    return 0;
}
