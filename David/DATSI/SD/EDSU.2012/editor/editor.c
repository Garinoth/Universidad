#include "editor.h"
#include "comun.h"
#include "edsu_comun.h"

int generar_evento( const char *tema, const char *valor ) {
    int sid;
    if ( ( sid = createSocket() ) < 0 ) {
        return -1;
    }

    int connection;
    if ( ( connection = connectToMediator( sid ) ) < 0 ) {
        return -1;
    }

    struct sockaddr_in dummy;
    sendMessage( sid, EVENT, tema, valor, dummy );

    Message message;
    recv( sid, &message, sizeof( message ), 0 );

    if ( message.op == OK ) {
        return 0;
    }
    else return -1;

    return 0;
}

/* solo para la version avanzada */
int crear_tema(const char *tema) {
	return 0;
}

/* solo para la version avanzada */
int eliminar_tema(const char *tema) {
	return 0;
}
