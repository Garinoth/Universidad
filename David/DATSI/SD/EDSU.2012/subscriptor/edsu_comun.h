/*
   Incluya en este fichero todas las definiciones que pueden
   necesitar compartir los módulos editor y subscriptor,
   si es que las hubiera.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int createSocket();
int connectToMediator();
void sendMessage( int sid, int op, const char *tema, const char *valor );