#include <pthread.h>
#include <stdbool.h>

typedef struct {
	int fd;
	pthread_t thread;
	bool free; // indica si la estructura esta libre par ser utilizada
} clientdata;

void cd_init(clientdata* clients, int len); // inicializa los bools en true
int cd_getFreeIndex(clientdata* clients, int len);
