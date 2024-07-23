all: writer reader

reader: reader.c common.c common.h
	gcc -std=gnu11 -Wall -pedantic reader.c common.c -o reader

writer: writer.c common.c common.h
	gcc -std=gnu11 -Wall -pedantic writer.c common.c -o writer