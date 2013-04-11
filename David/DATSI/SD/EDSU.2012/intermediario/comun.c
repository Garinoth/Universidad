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