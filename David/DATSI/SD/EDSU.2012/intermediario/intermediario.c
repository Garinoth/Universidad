#include "comun.h"


/* Structure that represents a them with an array of subscribers*/
typedef struct Theme {
    char name[ 128 ];
    struct sockaddr_in subscribers[ 128 ];
    int count;
} Theme;

/* Function declaration (move to .h) */
void addThemes ( char *themesFile );
void addTheme ( char *name );
Theme* searchTheme ( char *name, int *pos );
int startServer ( int port );
void sendResponse( int op, int sid );
int addSubscriber ( char *themeName, struct sockaddr_in address );
int removeSubscriber ( char *themeName, struct sockaddr_in address );
int notify ( char *themeName, char *event, int sid );
int searchSubscriber ( struct sockaddr_in address, Theme *theme );

/* DEBUG functions */
void printThemes ();

/* Global variables declaration */
Theme *themes[ 128 ]; // Stored themes
int themeCount = 0; // Count of stored themes

struct sockaddr_in sa; // Server address
socklen_t size_sa = sizeof( sa );

struct sockaddr ca; // Client address
socklen_t size_ca = sizeof( ca );

/* Main exectuion */
int main( int argc, char *argv[] ) {

    if ( argc!=3 ) {
        fprintf( stderr, "Uso: %s puerto fichero_temas\n", argv[ 0 ] );
        return 1;
    }

    int port = atoi( argv [ 1 ] );
    char *themesFile = argv[ 2 ];
    int sid;

    addThemes( themesFile );
    sid = startServer( port );

    int connection;
    while ( 1 ) {
        fprintf(stdout,"MEDIATOR: Waiting for request\n");

        if ( ( connection = accept( sid, ( struct sockaddr* )&ca, &size_ca ) ) < 0 ) {
            fprintf(stdout,"MEDIATOR: Request recieved: ERROR\n");
        }
        fprintf(stdout,"MEDIATOR: Request recieved: SUCCESS\n");

        Message message = recieveResponse( connection );

        switch ( message.op ) {
            case SUBSCRIBE :
                if ( addSubscriber( message.theme, message.saddr ) < 0 ) {
                    sendResponse( ERROR, connection );
                }
                else {
                    sendResponse( OK, connection );
                }
                break;
            
            case UNSUBSCRIBE :
                if ( removeSubscriber( message.theme, message.saddr ) < 0 ) {
                    sendResponse( ERROR, connection );
                }
                else {
                    sendResponse( OK, connection );
                }
                break;
            
            case EVENT :
                if ( notify( message.theme, message.value, sid ) < 0 ) {
                    sendResponse( ERROR, connection );
                }
                else {
                    sendResponse( OK, connection );
                }
                break;
            default :
                fprintf(stdout,"MEDIATOR: Unknown operation: ERROR\n");
        }
        close( connection );
    }


    return 0;
}


/* Server management */
int startServer ( int port ) {
    int sid;

    /* Socket creation */
    if ( ( sid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0){
        fprintf(stdout,"MEDIATOR: TCP socket creation : ERROR\n");
        exit(-1);
    }
    fprintf(stdout,"MEDIATOR: TCP socket creation : SUCCESS\n");

    /* Assign server port */
    bzero( ( char * )&sa, size_sa );
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons( port );
    if ( bind ( sid, ( struct sockaddr *)&sa, size_sa ) || getsockname ( sid, ( struct sockaddr * )&sa, &size_sa ) ) {
        fprintf( stdout,"MEDIATOR: Server port assignation: ERROR\n" );
        exit ( -1 );
    }
    fprintf( stdout,"MEDIATOR: Server port assignation: SUCCESS\n" );

    /* Listen from socket */
    if ( listen( sid, 0 ) < 0 ) {
        fprintf( stdout,"MEDIATOR: Request acceptation: ERROR\n" );
        exit( -1 );
    }
    fprintf( stdout,"MEDIATOR: Request acceptation: SUCCESS\n" );

    /* Puerto TCP ya disponible */
    fprintf( stdout,"MEDIATOR: TCP socket READY\n" );

    return sid;
}


void sendResponse( int op, int connection ) { // needs error handling
    Message message;
    message.op = op;

    int r;
    while ( ( r += write ( connection, &message, sizeof( message ) ) ) && r < sizeof( message ) );
}


/* Theme management */
void addThemes ( char *themesFile ) {
    FILE *file = fopen ( themesFile, "r" );

    if ( file != NULL ){
        char *line = malloc( sizeof( char ) * 128 );

        while ( fgets( line, sizeof line, file ) != NULL ) {
            line[ strlen( line ) - 1 ] = '\0'; // Quickfix to trim \n
            addTheme ( line );
        }
        fclose( file );
    }
    else perror( themesFile );
}


void addTheme ( char *name ) {
    Theme *newTheme = malloc( sizeof( Theme ) );
    strcpy( newTheme->name, name );
    newTheme->count = 0;

    themes[ themeCount++ ] = newTheme;

    fprintf( stdout,"MEDIATOR: Added new theme: %s\n", name );
}


Theme* searchTheme ( char *name, int *pos ) {
    Theme *theme = NULL;
    int i = 0;
    while ( ( i < themeCount ) && ( theme == NULL ) ) {
        if ( !strcmp( name, themes[ i ]->name ) ) {
            theme = themes[ i ];
            if ( pos ) *pos = i;
        }
        i++;
    }

    return theme;
}


/* Subscription management */
int addSubscriber ( char *themeName, struct sockaddr_in address ) {
    Theme *theme;
    if ( ( theme = searchTheme( themeName, NULL ) ) == NULL ) {
        return -1;
    }

    theme->subscribers[ theme->count++ ] = address;

    return 0;
}

int removeSubscriber ( char *themeName, struct sockaddr_in address ) {
    Theme *theme;
    if ( ( theme = searchTheme( themeName, NULL ) ) == NULL ) {
        return -1;
    }

    int pos;
    if ( ( pos = searchSubscriber( address, theme ) ) < 0 ) {
        return -1;
    }

    int i;
    for ( i = pos; i < theme->count; i++ ) {
        theme->subscribers[ i ] = theme->subscribers[ i + 1 ];
    }
    theme->count--;
    return 0;
}

int notify ( char *themeName, char *event, int sid ) {
    Theme *theme;
    if ( ( theme = searchTheme( themeName, NULL ) ) == NULL ) {
        return -1;
    }

    int i;
    for ( i = 0; i < theme->count; i++ ) {

        int sid;
        sid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        int connection;
        if( ( connection = connect( sid, ( struct sockaddr* )&theme->subscribers[ i ], sizeof( struct sockaddr_in ) ) ) < 0 ) {
            close(sid); 
        }

        struct sockaddr_in dummy;
        sendMessage( sid, EVENT, themeName, event, dummy );
    }
    return 0;
}

int searchSubscriber ( struct sockaddr_in address, Theme *theme ) {
    int i = 0;
    int res = -1;

    while ( ( i < theme->count ) && ( res == -1 ) ) {        
        if ( address.sin_addr.s_addr == theme->subscribers[ i ].sin_addr.s_addr ) {
            res = i;
        }
        i++;
    }
    return res;
}


/* DEBUG functions */
void printThemes () {
    if ( themeCount == 0 ) {
        printf ( "NO THEMES" );
    }

    int i;
    for ( i = 0; i < themeCount; i++ ) {
        printf( "%s\n", themes[ i ]->name );
    }
}
