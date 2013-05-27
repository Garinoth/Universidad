/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#include "comun.h"

Message recieveResponse( int connection ) {
    Message message;
    int r;
    while ( ( r += read ( connection, &message, sizeof( message ) ) ) && r < sizeof( message ) );
    return message;
}

void sendMessage( int sid, int op, const char *tema, const char *valor, struct sockaddr_in saddr ) {
    Message message;
    message.op = op;
    if ( tema ) strcpy( message.theme, tema );
    if ( valor ) strcpy( message.value, valor );
    message.saddr = saddr;

    send( sid, &message, sizeof( message ), 0 );
}