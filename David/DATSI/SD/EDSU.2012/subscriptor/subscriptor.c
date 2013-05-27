#include <pthread.h>

#include "subscriptor.h"
#include "comun.h"
#include "edsu_comun.h"


void (*notify)(const char *, const char *);
void *subscribe;
void *unsubscribe;

int sid_ev;
int ev_port;
struct sockaddr_in ca;
socklen_t size_ca;
pthread_t thread;

void *recieveEvents();

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
    
    sendMessage( sid, SUBSCRIBE, tema, NULL, ca );

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
    
    sendMessage( sid, UNSUBSCRIBE, tema, NULL, ca );

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
    if ( ( sid_ev = createSocket() ) < 0 ) {
        return -1;
    }

    size_ca = sizeof( ca );

    bzero( ( char * )&ca, size_ca );
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = INADDR_ANY;
    ca.sin_port = htons( 0 );

    while ( bind ( sid_ev, ( struct sockaddr *)&ca, size_ca ) || getsockname ( sid_ev, ( struct sockaddr * )&ca, &size_ca ) ) {
        return -1;
    }

    if ( listen( sid_ev, 0 ) < 0 ) {
        return -1 ;
    }

    pthread_create( &thread, NULL, &recieveEvents, NULL );

    return 0;
}

void *recieveEvents( void *arg ) {
    int connection;
    while ( 1 ) {
        struct sockaddr sa;
        socklen_t size_sa = sizeof( sa );
        if ( ( connection = accept( sid_ev, ( struct sockaddr* )&sa, &size_sa ) ) >= 0 ) {
            Message message = recieveResponse( connection );
            if ( message.op == EVENT ) {
                (*notify)( message.theme, message.value );
            }
        }
    }
    close( connection );
    return NULL;
}

int fin_subscriptor() {
    return 0;
}

