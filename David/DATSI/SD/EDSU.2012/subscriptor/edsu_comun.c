/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir los módulos editor y subscriptor,
   si es que las hubiera.
*/

#include "edsu_comun.h"
#include "comun.h"

int createSocket() {
    int sid;
    if ( ( sid = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        return -1;
    }
    return sid;
}

int connectToMediator( int sid ) {
    char *host = getenv( "SERVIDOR" );
    int port = atoi ( getenv( "PUERTO" ) );

    struct sockaddr_in sa;
    bzero( ( char* )&sa, sizeof( struct sockaddr_in ) );
    sa.sin_family=AF_INET;
    sa.sin_addr.s_addr=inet_addr( host );
    sa.sin_port=htons( port );

    int connection;
    if( ( connection = connect( sid, ( struct sockaddr* )&sa, sizeof( struct sockaddr_in ) ) ) < 0 ) {
        close(sid); 
        return -1;    
    }
    return connection;
}

void sendMessage( int sid, int op, const char *tema, const char *valor ) {
    Message message;
    message.op = op;
    if ( tema ) strcpy( message.theme, tema );
    if ( valor ) strcpy( message.value, valor );

    send( sid, &message, sizeof( message ), 0 );
}