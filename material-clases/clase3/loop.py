import time
while True:
    time.sleep(1)
    print("tick")

"""Ejecutar el script.
 python loop.py

Se deberá observar en la consola los mensajes “tick” cada un segundo.

Ejecutar la lista de procesos activos para ver el estado del mismo y obtener el PID.

 ps -elf

Aparecerá el proceso con una S (Interruptable) debido a la función sleep(). Obtener el PID.

Presionar Ctrl+Z para enviar la signal SIGSTP al proceso.

Ejecutar la lista de procesos activos para ver el estado.

 ps -elf

Aparecerá el proceso con una T (stopped).

Reanudar el proceso enviando la señal de continuar:

 kill -SIGCONT pid

Se deberá ver el "tick" nuevamente."""

