#include "subscriptor.h"
#include "comun.h"
#include "edsu_comun.h"

void *notify;
void *subscribe;
void *unsubscribe;


int alta_subscripcion_tema(const char *tema) {
    if ( !notify ) {
        return -1;
    }

    int sid;
    if ( ( sid = createSocket() ) < 0 ) {
        return -1;
    }

    int connection;
    if ( ( connection = connectToMediator( sid ) ) < 0 ) {
        return -1;
    }
    
    sendMessage( sid, SUBSCRIBE, tema, NULL );

    Message message;
    recv( sid, &message, sizeof( message ), 0 );

    if ( message.op == OK ) {
        return 0;
    }
    else return -1;
}

int baja_subscripcion_tema(const char *tema) {
    if ( !notify ) {
        return -1;
    }

    int sid;
    if ( ( sid = createSocket() ) < 0 ) {
        return -1;
    }

    int connection;
    if ( ( connection = connectToMediator( sid ) ) < 0 ) {
        return -1;
    }
    
    sendMessage( sid, UNSUBSCRIBE, tema, NULL );

    Message message;
    recv( sid, &message, sizeof( message ), 0 );

    if ( message.op == OK ) {
        return 0;
    }
    else return -1;
}

int inicio_subscriptor(void (*notif_evento)(const char *, const char *),
                void (*alta_tema)(const char *),
                void (*baja_tema)(const char *)) {

    notify = notif_evento;
    subscribe = alta_tema;
    unsubscribe = baja_tema;

    return 0;
}

int fin_subscriptor() {
    return 0;
}

