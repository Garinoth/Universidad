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


#define OK          0
#define ERROR       1
#define SUBSCRIBE   2  
#define UNSUBSCRIBE 3  
#define EVENT       4  

typedef struct Message {
  	int  op;
  	int  port;
  	char theme [ 128 ];
  	char value [ 256 ];
    struct sockaddr_in saddr;
} Message;

Message recieveResponse( int connection );
void sendMessage( int sid, int op, const char *tema, const char *valor, struct sockaddr_in saddr );