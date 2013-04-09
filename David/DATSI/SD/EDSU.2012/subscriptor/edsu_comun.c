/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir los módulos editor y subscriptor,
   si es que las hubiera.
*/

#include "edsu_comun.h"

int connectToMediator () {
	int sid;
	struct sockaddr_in sa;

	char *host = getenv( "SERVIDOR" );
	int port = atoi ( getenv( "PUERTO" ) );

	if ( ( sid = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
		fprintf(stdout,"CLIENT:  TCP socket creation: ERROR\n");
		exit( 1 );
	}

	bzero( ( char* )&sa, sizeof( struct sockaddr_in ) );
	sa.sin_family=AF_INET;
  	sa.sin_addr.s_addr=inet_addr( host );
  	sa.sin_port=htons( port );

  	if( connect( sid, ( struct sockaddr* )&sa, sizeof( struct sockaddr_in ) ) < 0 ) {
      	fprintf(stdout,"CLIENT:  Connection to mediator: ERROR\n");
      	close(sid); 
      	exit(1);
    }
    return sid;
}