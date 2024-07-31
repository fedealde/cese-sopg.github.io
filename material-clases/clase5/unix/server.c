#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

//ES UN ECHO SERVER, devuelve lo que recibe

#define SOCKET_PATH "/tmp/mysocket" 
//ruta a archivo comun y corriente, tenemos que tiene nombre y este es mysocket

int main() {
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        return 1;
    }

// aca borro el inodo o archivo por si existe, evitamos el problema
    unlink(SOCKET_PATH); // avoid "address already in use"

    struct sockaddr_un local = {0}; // _un es de unix
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCKET_PATH);
    // así se especifica la longitud, why? i dont know
    int addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(local.sun_path) + 1;
    //con este asignamos y mas abajo hacemos el listen
    if (bind(s, (struct sockaddr *)&local, addrlen) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(s, 10) != 0) {
        perror("listen");
        return 1;
    }

    while (true) {// todo es igual que el de internet excepto que es para unix
        printf("waiting for connection....\n");

        unsigned int sock_len;
        struct sockaddr_un remote;
        int conn = accept(s, (struct sockaddr *)&remote, &sock_len);
        if (conn  == -1) {
            perror("accept");
            return 1;
        }

        printf("server connected\n");

        while (true) {
            char buf[100] = {0};
            int n = recv(conn, buf, 100, 0);
            if (n < 0) {
                perror("recv");
                return 1;
            }
            if (n == 0) { // EOF
                break;
            }
            buf[n] = '\0';
            printf("received %d bytes from client: %s\n", n, buf);

            // echo to client
            if (send(conn, buf, strlen(buf), 0) == -1) {
                perror("send");
                return 1;
            }
        }

        close(conn);
    }

    return 0;
}
