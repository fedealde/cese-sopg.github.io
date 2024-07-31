# Trabajo práctico
Autor: Federico Alderisi

## Writer 

1.  En el proceso Writer se manejan los errores de mknod, open, close, read, write al utilizar file descriptors (sin incluir STDIN_FILENO). Además EOF (ya sea porque Reader no existe más e intento escribir o presiono Ctrl+D)
2. Se manejan las señales SIGUSR1 y SIGUSR2 como describe la consigna. Además se maneja SIGINT.
3. Los mensajes son enviados a la FIFO, utilizando el encabezado "DATA: " para datos ingresados por STDIN_FILENO y "SIGN: " para el manejo de las señales. El máximo de caracteres que pueden ser ingresados esta dado por FIFO_MSG_SIZE (=21) teniendo en cuenta que se agrega '\0' al final del dato (quedarían solo 20 caracteres para el dato).
4. Cuando se escribe un mensaje más largo que el que esta por defecto, se parte y envía por separado, por lo que no genera un desbordamiento.
5. Antes de enviar los mensajes se agrega '\0' para evitar saltos de linea en printf o que sean enviados a la FIFO
6. Ejemplos de prueba:
    -   12345678901234567890123456789012345
    -    kill -SIGUSR1 24399
    -    kill -SIGUSR2 24399

## Reader 

1.  En el proceso reader se manejan los errores de mknod, open, close, read, write al utilizar file descriptors. Además EOF cuando no hay más nada para recibir porque no hay Writer o llegó al EOF.
2. Se maneja SIGINT.
3. Los mensajes son recibidos de la FIFO, se les quita el encabezado "DATA: " para datos ingresados por STDIN_FILENO y "SIGN: " para el manejo de las señales que fue realizado en el Writer. Esto se hace comparando los encabezados y tomando los datos que continúan, a menos que el encabezado no sea correcto (aquí se descarta el mensaje imprimiendo un mensaje de error). El máximo de caracteres que pueden ser leídos esta dado por FIFO_MSG_TO_SEND_SIZE (=27) teniendo en cuenta que se agrega '\0' al final del dato (quedarían solo 20 caracteres para el dato + '\n' que se utiliza para el Log.txt o el Sign.txt).
4. Luego de enviar los mensajes se agrega '\0' para evitar saltos de linea en printf final luego de la escritura en el .txt

## Compilación

borrar todos los .txt y ejecutables: make clean
compilar todo: make all