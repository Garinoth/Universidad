/*
   Incluya en este fichero todas las definiciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SUBSCRIBE   1  
#define UNSUBSCRIBE 2  
#define EVENT       3  

typedef struct Message {
  	int  op;
  	int  port;
  	char theme [ 128 ];
  	char value [ 256 ];
} Message;