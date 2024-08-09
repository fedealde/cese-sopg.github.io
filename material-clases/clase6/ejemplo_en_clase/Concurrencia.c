#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int a;
    int b;
} Data;

Data data;

void *start_thread(void *message) {

    while (1) {
        data.a++;
        data.b++;
        usleep(100);
    }
}

int main(void) {

    pthread_t t1, t2;
    pthread_create(&t1, NULL, start_thread, NULL);
    pthread_create(&t2, NULL, start_thread, NULL);

    while (1) {
        int a = data.a;
        int b = data.b;
        printf("%d %d\n", a, b);
        sleep(1);
    }
}

// Uno espera que a y b tengan el mismo valor. Por condicion de carrera puede
// cambiar hasta al ejecutar solo un hilo además del main al momento de
// mostrarlo